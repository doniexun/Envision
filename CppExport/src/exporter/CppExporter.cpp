/***********************************************************************************************************************
 **
 ** Copyright (c) 2011, 2015 ETH Zurich
 ** All rights reserved.
 **
 ** Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 ** following conditions are met:
 **
 **    * Redistributions of source code must retain the above copyright notice, this list of conditions and the
 **      following disclaimer.
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
 **********************************************************************************************************************/

#include "CppExporter.h"

#include "OOModel/src/declarations/Project.h"
#include "OOModel/src/expressions/MetaCallExpression.h"
#include "OOModel/src/declarations/TypeAlias.h"

#include "Export/src/writer/Exporter.h"
#include "Export/src/writer/FragmentLayouter.h"
#include "Export/src/tree/CompositeFragment.h"
#include "ModelBase/src/model/TreeManager.h"
#include "OOModel/src/declarations/NameImport.h"
#include "OOModel/src/declarations/ExplicitTemplateInstantiation.h"
#include "OOInteraction/src/string_offset_providers/StringComponents.h"

#include "../CodeUnit.h"
#include "../CodeComposite.h"
#include "../Config.h"

namespace CppExport {

QList<Export::ExportError> CppExporter::exportTree(Model::TreeManager* treeManager,
																	const QString& pathToProjectContainerDirectory)
{
	QList<CodeUnit*> codeUnits;
	units(treeManager->root(), "", codeUnits);

	for (auto unit : codeUnits) unit->calculateSourceFragments();
	for (auto unit : codeUnits) unit->calculateDependencies(codeUnits);

	auto directory = new Export::SourceDir{nullptr, pathToProjectContainerDirectory};
	for (auto codeComposite : mergeUnits(codeUnits))
		createFilesFromComposite(directory, codeComposite);

	auto layout = layouter();
	Export::Exporter::exportToFileSystem("", directory, &layout);

	return {};

}

void CppExporter::createFilesFromComposite(Export::SourceDir* directory, CodeComposite* codeComposite)
{
	Export::SourceFragment* headerFragment{};
	Export::SourceFragment* sourceFragment{};
	codeComposite->fragments(headerFragment, sourceFragment);

	if (headerFragment) directory->file(codeComposite->name() + ".h").append(headerFragment);
	if (sourceFragment) directory->file(codeComposite->name() + ".cpp").append(sourceFragment);
}

void CppExporter::units(Model::Node* current, QString namespaceName, QList<CodeUnit*>& result)
{
	if (!DCast<OOModel::Project>(current))
	{
		if (auto ooModule = DCast<OOModel::Module>(current))
		{
			// ignore the "ExternalMacro" module
			if (ooModule->name() == "ExternalMacro") return;

			namespaceName = ooModule->name();
		}
		else if (auto ooNameImport = DCast<OOModel::NameImport>(current))
		{
			auto referenceExpression = DCast<OOModel::ReferenceExpression>(ooNameImport->importedName());
			Q_ASSERT(referenceExpression);

			if (!referenceExpression->target() || !DCast<OOModel::Project>(referenceExpression->target()))
				result.append(new CodeUnit{(namespaceName.isEmpty() ? "" : namespaceName + "/") +
													referenceExpression->name(), current});
			return;
		}
		else if (auto ooExplicitTemplateInstantiation = DCast<OOModel::ExplicitTemplateInstantiation>(current))
		{
			result.append(new CodeUnit{(namespaceName.isEmpty() ? "" : namespaceName + "/") +
												OOInteraction::StringComponents::stringForNode(
													ooExplicitTemplateInstantiation->instantiatedClass()), current});
			return;
		}
		else if (auto ooDeclaration = DCast<OOModel::Declaration>(current))		{
			result.append(new CodeUnit{(namespaceName.isEmpty() ? "" : namespaceName + "/") +
												ooDeclaration->name(), current});
			return;
		}
		else if (auto ooMetaCall = DCast<OOModel::MetaCallExpression>(current))
		{
			auto ooCalleeReference = DCast<OOModel::ReferenceExpression>(ooMetaCall->callee());
			Q_ASSERT(ooCalleeReference);

			QStringList arguments;
			for (auto argument : *ooMetaCall->arguments())
				arguments.append(OOInteraction::StringComponents::stringForNode(argument));

			result.append(new CodeUnit{(namespaceName.isEmpty() ? "" : namespaceName + "/") + ooCalleeReference->name() +
												"(" + arguments.join(",") + ")", current});
			return;
		}
	}

	for (auto child : current->children())
		units(child, namespaceName, result);
}

QList<CodeComposite*> CppExporter::mergeUnits(QList<CodeUnit*>& units)
{
	QHash<QString, QString> mergeMap = Config::instance().dependencyUnitMergeMap();

	QHash<QString, CodeComposite*> nameToCompositeMap;
	for (auto unit : units)
	{
		auto it = mergeMap.find(unit->name());
		auto compositeName = it != mergeMap.end() ? *it : unit->name();

		auto cIt = nameToCompositeMap.find(compositeName);
		if (cIt != nameToCompositeMap.end())
			// case A: the composite that unit is a part of already exists => merge
			(*cIt)->addUnit(unit);
		else
		{
			// case B: the composite that unit is a part of does not yet exist
			auto composite = new CodeComposite{compositeName};
			composite->addUnit(unit);
			nameToCompositeMap.insert(composite->name(), composite);
		}
	}

	return nameToCompositeMap.values();
}

Export::FragmentLayouter CppExporter::layouter()
{
	auto result = Export::FragmentLayouter{"\t"};
	result.addRule("enumerators", Export::FragmentLayouter::IndentChildFragments, "", ",\n", "");
	result.addRule("vertical", Export::FragmentLayouter::NoIndentation, "", "\n", "");
	result.addRule("sections", Export::FragmentLayouter::NoIndentation, "", "\n", "");
	result.addRule("declarationComment", Export::FragmentLayouter::NoIndentation
							| Export::FragmentLayouter::NewLineAfterPostfix, "", "\n", "");
	result.addRule("spacedSections", Export::FragmentLayouter::NoIndentation, "", "\n\n", "");
	result.addRule("accessorSections", Export::FragmentLayouter::IndentChildFragments, "", "\n", "");
	result.addRule("bodySections", Export::FragmentLayouter::NewLineBefore
						  | Export::FragmentLayouter::IndentChildFragments | Export::FragmentLayouter::NewLineAfterPrefix
						  | Export::FragmentLayouter::NewLineBeforePostfix, "{", "\n", "}");
	result.addRule("space", Export::FragmentLayouter::SpaceAtEnd, "", " ", "");
	result.addRule("comma", Export::FragmentLayouter::SpaceAfterSeparator, "", ",", "");
	result.addRule("initializerList", Export::FragmentLayouter::SpaceAfterSeparator, "{", ",", "}");
	result.addRule("argsList", Export::FragmentLayouter::SpaceAfterSeparator, "(", ",", ")");
	result.addRule("typeArgsList", Export::FragmentLayouter::SpaceAfterSeparator, "<", ",", ">");
	result.addRule("templateArgsList", Export::FragmentLayouter::SpaceAfterSeparator
							| Export::FragmentLayouter::NewLineAfterPostfix, "template<", ",", ">");

	result.addRule("body", Export::FragmentLayouter::NewLineBefore | Export::FragmentLayouter::IndentChildFragments
							| Export::FragmentLayouter::NewLineAfterPrefix | Export::FragmentLayouter::NewLineBeforePostfix,
							"{", "\n", "}");
	result.addRule("bodyNoBraces", Export::FragmentLayouter::IndentChildFragments
							| Export::FragmentLayouter::NewLineAfterPrefix | Export::FragmentLayouter::NewLineBeforePostfix,
							"", "\n", "");
	result.addRule("sameLineIf", Export::FragmentLayouter::NewLineBeforePostfix, " ", "\n", "");
	result.addRule("macroBody", Export::FragmentLayouter::NewLineBefore | Export::FragmentLayouter::IndentChildFragments
							| Export::FragmentLayouter::NewLineAfterPrefix | Export::FragmentLayouter::NewLineBeforePostfix,
							"", "\n", "");
	result.addRule("macro", Export::FragmentLayouter::BackslashAfterLines
						| Export::FragmentLayouter::NewLineAfterPrefix | Export::FragmentLayouter::NewLineBeforePostfix);
	result.addRule("emptyLineAtEnd", Export::FragmentLayouter::EmptyLineAtEnd);
	result.addRule("namespace", Export::FragmentLayouter::NoIndentation | Export::FragmentLayouter::SpaceAfterPrefix,
						"namespace", "", "");

	return result;
}

Export::ExportMapContainer& CppExporter::exportMaps()
{
	static Export::ExportMapContainer* container = new Export::ExportMapContainer{};
	return *container;
}

}
