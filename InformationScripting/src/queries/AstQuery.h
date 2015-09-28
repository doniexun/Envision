/***********************************************************************************************************************
**
** Copyright (c) 2011, 2015 ETH Zurich
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
** following conditions are met:
**
**    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following
**      disclaimer.
**    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
**      following disclaimer in the documentation and/or other materials provided with the distribution.
**    * Neither the name of the ETH Zurich nor the names of its contributors may be used to endorse or promote products
**      derived from this software without specific prior written permission.
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

#pragma once

#include "../informationscripting_api.h"

#include "ScopedArgumentQuery.h"

namespace Model {
	class Node;
	class SymbolMatcher;
}

namespace OOModel {
	class Class;
	class Method;
}

namespace InformationScripting {

class INFORMATIONSCRIPTING_API AstQuery : public ScopedArgumentQuery
{
	public:
		virtual QList<TupleSet> execute(QList<TupleSet> input) override;

		static void registerDefaultQueries();

	private:

		static const QStringList NODETYPE_ARGUMENT_NAMES;
		static const QStringList NAME_ARGUMENT_NAMES;
		static const QStringList ADD_AS_NAMES;

		ExecuteFunction<AstQuery> exec_{};

		AstQuery(ExecuteFunction<AstQuery> exec, Model::Node* target, QStringList args);

		static void setTypeTo(QStringList& args, QString type);

		QList<TupleSet> baseClassesQuery(QList<TupleSet> input);
		QList<TupleSet> toParentType(QList<TupleSet> input);
		QList<TupleSet> callGraph(QList<TupleSet> input);
		QList<TupleSet> genericQuery(QList<TupleSet> input);
		QList<TupleSet> typeQuery(QList<TupleSet> input, QString type);
		QList<TupleSet> nameQuery(QList<TupleSet> input, QString name);
		QList<TupleSet> usesQuery(QList<TupleSet> input);

		void addBaseEdgesFor(OOModel::Class* childClass, NamedProperty& classNode, TupleSet& ts);
		void addNodesOfType(TupleSet& ts, const Model::SymbolMatcher& matcher, Model::Node* from = nullptr);
		void addCallInformation(TupleSet& ts, OOModel::Method* method, QList<OOModel::Method*> callees);

		void adaptOutputForRelation(TupleSet& tupleSet, const QString& relationName, const QStringList& keepProperties);
};

} /* namespace InformationScripting */