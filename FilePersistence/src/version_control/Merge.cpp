/***********************************************************************************************************************
**
** Copyright (c) 2011, 2015 ETH Zurich
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
** following conditions are met:
**
** * Redistributions of source code must retain the above copyright notice, this list of conditions and the following
** disclaimer.
** * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
** following disclaimer in the documentation and/or other materials provided with the distribution.
** * Neither the name of the ETH Zurich nor the names of its contributors may be used to endorse or promote products
** derived from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
** INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
** SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
** WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
***********************************************************************************************************************/

#include "Merge.h"

#include "GitRepository.h"

#include "ModelBase/src/model/TreeManager.h"

namespace FilePersistence {

bool Merge::abort()
{
	if (stage_ != Stage::NoMerge && stage_ != Stage::Complete)
	{
		QString branch = repository_->currentBranch();

		repository_->checkout(headCommitId, true);
		repository_->setReferenceTarget(branch, headCommitId);

		stage_ = Stage::NoMerge;
		return true;
	}
	else
		return false;
}

bool Merge::commit(const Signature& author, const Signature& committer, const QString& message)
{
	if (stage_ == Stage::ReadyToCommit)
	{
		QString treeSHA1 = repository_->writeIndexToTree();

		QStringList parents;
		parents.append(headCommitId);
		parents.append(revisionCommitId_);

		repository_->newCommit(treeSHA1, message, author, committer, parents);

		stage_ = Stage::Complete;

		return true;
	}
	else
		return false;
}

// ======== private ========

Merge::Merge(QString revision, bool fastForward, GitRepository* repository)
	: fastForward_{fastForward}, repository_{repository}
{
	headCommitId = repository_->getSHA1("HEAD");
	revisionCommitId_ = repository_->getSHA1(revision);
	baseCommitId_ = repository_->findMergeBase("HEAD", revision);

	if (baseCommitId_.isNull())
		error_ = Error::NoMergeBase;

	stage_ = Stage::Initialized;

	if (baseCommitId_.compare(revisionCommitId_) == 0)
	{
		kind_ = Kind::AlreadyUpToDate;
	}
	else if (baseCommitId_.compare(headCommitId) == 0)
	{
		kind_ = Kind::FastForward;
	}
	else
		kind_ = Kind::TrueMerge;

	stage_ = Stage::Classified;

	switch (kind_)
	{
		case Kind::AlreadyUpToDate:
			stage_ = Merge::Stage::Complete;
			break;

		case Kind::FastForward:
			if (fastForward_)
			{
				QString branch = repository_->currentBranch();
				repository_->setReferenceTarget(branch, revisionCommitId_);
				repository_->checkout(revisionCommitId_, true);
				stage_ = Stage::Complete;
			}
			else
			{
				repository_->checkout(revisionCommitId_, true);
				repository_->writeRevisionIntoIndex(revisionCommitId_);
				stage_ = Stage::ReadyToCommit;
			}
			break;

		case Kind::TrueMerge:
			performTrueMerge();
			break;

		default:
			Q_ASSERT(false);
	}
}

void Merge::performTrueMerge()
{
		Diff diffA = repository_->diff(baseCommitId_, headCommitId);
		Diff diffB = repository_->diff(baseCommitId_, revisionCommitId_);

		ChangeDependencyGraph cdgA = ChangeDependencyGraph(diffA);
		ChangeDependencyGraph cdgB = ChangeDependencyGraph(diffB);
		conflictingChanges_ = {};
		conflictPairs_ = {};

		treeA_ = std::unique_ptr<GenericTree>(new GenericTree("HeadTree", headCommitId));
		repository_->loadGenericTree(treeA_, headCommitId);

		treeB_ = std::unique_ptr<GenericTree>(new GenericTree("MergeRevision", revisionCommitId_));
		repository_->loadGenericTree(treeB_, revisionCommitId_);

		treeBase_ = std::unique_ptr<GenericTree>(new GenericTree("MergeBase", baseCommitId_));
		repository_->loadGenericTree(treeBase_, baseCommitId_);

		for (auto component : conflictPipeline_)
		{
			component->run(treeBase_, treeB_, treeA_, cdgA, cdgB, conflictingChanges_, conflictPairs_);
		}

		IdToChangeDescriptionHash applicableChanges;
		for (auto change : cdgA.changes())
			if (!conflictingChanges_.contains(change)) applicableChanges.insert(change->id(), change);
		for (auto change : cdgB.changes())
			if (!conflictingChanges_.contains(change)) applicableChanges.insert(change->id(), change);

		//And then do manual merge
}

} /* namespace FilePersistence */
