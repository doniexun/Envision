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

#include "Graph.h"

#include "InformationNode.h"
#include "InformationEdge.h"

namespace InformationScripting {

QList<Graph::IsEqual> Graph::equalityChecks_;

Graph::~Graph()
{
	std::for_each(edges_.begin(), edges_.end(), [](InformationEdge* e) { SAFE_DELETE(e); });
	std::for_each(nodes_.begin(), nodes_.end(), [](InformationNode* n) { SAFE_DELETE(n); });
}

InformationNode* Graph::add(InformationNode* node)
{
	InformationNode* existingNode = nullptr;
	for (auto graphNode : nodes_)
	{
		for (auto checker : equalityChecks_)
		{
			if (checker(graphNode, node))
			{
				existingNode = graphNode;
				break;
			}
		}
		if (existingNode) break;
	}
	if (existingNode)
	{
		// TODO merge
		// TODO delete argument node
		node = existingNode;
		Q_ASSERT(false);
	}
	else
	{
		nodes_.push_back(node);
	}
	return node;
}

InformationEdge* Graph::addDirectedEdge(InformationNode* from, InformationNode* to, const QString& name)
{
	// TODO what if from/to is not yet in the graph?
	for (auto edge : edges_)
	{
		if (edge->from() == from && edge->to() == to && edge->name() == name)
		{
			auto existingEdge = edge;
			Q_ASSERT(existingEdge->isDirected());
			existingEdge->incrementCount();
			return existingEdge;
		}
	}
	auto edge = new InformationEdge(from, to, name);
	edges_.push_back(edge);
	return edge;
}

InformationEdge* Graph::addEdge(InformationNode* a, InformationNode* b, const QString& name)
{
	// TODO what if from/to is not yet in the graph?
	for (auto edge : edges_)
	{
		if ((edge->from() == a || edge->from() == b) && (edge->to() == b || edge->to() == a) && edge->name() == name)
		{
			auto existingEdge = edge;
			Q_ASSERT(!existingEdge->isDirected());
			existingEdge->incrementCount();
			return existingEdge;
		}
	}
	auto edge = new InformationEdge(a, b, name, InformationEdge::Orientation::Undirected);
	edges_.push_back(edge);
	return edge;
}

void Graph::remove(InformationNode* node)
{
	for (auto edgeIt = edges_.begin(); edgeIt != edges_.end();)
	{
		auto edge = *edgeIt;
		if (edge->from() == node || edge->to() == node)
			edgeIt = edges_.erase(edgeIt);
		else
			++edgeIt;
	}

	auto nodeIt = std::find(nodes_.begin(), nodes_.end(), node);
	nodes_.erase(nodeIt);
}

void Graph::remove(QList<InformationNode*> nodes)
{
	for (auto node : nodes) remove(node);
}

QList<InformationNode*> Graph::nodesForWhich(Graph::NodeCondition holds) const
{
	Q_ASSERT(holds);

	QList<InformationNode*> result;
	for (auto node : nodes_) if (holds(node)) result.push_back(node);
	return result;
}

QList<InformationEdge*> Graph::edgesFowWhich(Graph::EdgeCondition holds) const
{
	Q_ASSERT(holds);

	QList<InformationEdge*> result;
	for (auto edge : edges_) if (holds(edge)) result.push_back(edge);
	return result;
}

} /* namespace InformationScripting */
