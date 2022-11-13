#include <rt/groups/bvh.h>
#include <algorithm>
#include <iostream>
#include <stack> 

namespace rt {

BVH::BVH()
{
    /* TODO */
}

void BVH::storePrimitiveInfo(std::vector<BVHPrimitiveInfo> &primitiveInfo) {
	
	for (Primitive* p : primitives) {
			primitiveInfo[primNumi].bounds = p->getBounds();
			primitiveInfo[primNumi].centroid.x = (primitiveInfo[primNumi].bounds.min.x + primitiveInfo[primNumi].bounds.max.x) / 2;
			primitiveInfo[primNumi].centroid.y = (primitiveInfo[primNumi].bounds.min.y + primitiveInfo[primNumi].bounds.max.y) / 2;
			primitiveInfo[primNumi].centroid.z = (primitiveInfo[primNumi].bounds.min.z + primitiveInfo[primNumi].bounds.max.z) / 2;
			primitiveInfo[primNumi].primitiveNumber = primNumi;
			primNumi++;
	}
}

void BVH::rebuildIndex() {
	std::vector<BVHPrimitiveInfo> primitiveInfo(primitives.size());
	storePrimitiveInfo(primitiveInfo);
	int totalNodes = 0;
	Primitives odrPrims;
	BVHBuildNode * root =  (BVHBuildNode *)malloc(sizeof(BVHBuildNode));
	root = recursiveBuild(primitiveInfo, 0, primitives.size(),
			&totalNodes, odrPrims);
	std::cout << "size::" << primitives.size() << "\n" << std::endl;
	std::cout << "totalNodes::" << totalNodes << "\n" << std::endl;

	primitives.swap(odrPrims);
	nodes = (LinearBVHNode *)malloc(totalNodes *sizeof(LinearBVHNode));
	int offset = 0;
	createLinearTree(root, &offset);
}

int BVH::createLinearTree(BVHBuildNode *node, int *offset) {
	LinearBVHNode *linearNode = &nodes[*offset];
	linearNode->bounds = node->bounds;

	int myOffset = (*offset)++;
	if (node->nPrimitives > 0) {
		linearNode->primitivesOffset = node->firstPrimOffset;
		linearNode->nPrimitives = node->nPrimitives;
	}
	else {
		linearNode->axis = node->splitAxis;
		linearNode->nPrimitives = 0;
		createLinearTree(node->children[0], offset);
		linearNode->secondChildOffset =
			createLinearTree(node->children[1], offset);

	}
	return myOffset;
}

BVHBuildNode*  BVH::recursiveBuild(std::vector<BVHPrimitiveInfo> &primitiveInfo, int start, int end,
	int *totalNodes, Primitives &orderedPrims) {
	
	BVHBuildNode * node = (BVHBuildNode *)malloc(sizeof(BVHBuildNode));
	(*totalNodes)++;
	BBox bounds;
	bounds = bounds.empty();
	for (int i = start; i < end; ++i) {
		bounds.extend(primitiveInfo[i].bounds);
	}
	int nPrimitives = end - start;
	if (nPrimitives == 1) {
		// Create Leaf Node
		int firstPrimOffset = orderedPrims.size();
		for (int i = start; i < end; ++i) {
			int primNum = primitiveInfo[i].primitiveNumber;
			Primitive* p = primitives[primNum];
			orderedPrims.push_back(p);
		}
		node->InitLeaf(firstPrimOffset, nPrimitives, bounds);

		return node;
	}
	else {
		BBox centroidBounds;
		centroidBounds = centroidBounds.empty();
		for (int i = start; i < end; ++i) {
			centroidBounds.extend(primitiveInfo[i].centroid);
		}
		int dim = MaximumExtent(centroidBounds);
		float maxValue, minValue, centroidValue;
		switch (dim) {
		case 0:
			maxValue = centroidBounds.max.x;
			minValue = centroidBounds.min.x;
			break;
		case 1:
			maxValue = centroidBounds.max.y;
			minValue = centroidBounds.min.y;
			break;
		case 2:
			maxValue = centroidBounds.max.z;
			minValue = centroidBounds.min.z;
			break;
		default:
			break;
		}
		int mid = (start + end) / 2;

		if (maxValue == minValue) {
			// Create leaf BVHBuildNode 
				int firstPrimOffset = orderedPrims.size();
			for (int i = start; i < end; ++i) {
				int primNum = primitiveInfo[i].primitiveNumber;
				orderedPrims.push_back(primitives[primNum]);
			}
			node->InitLeaf(firstPrimOffset, nPrimitives, bounds);
			return node;
		}
		else {
			// Partition primitives based on splitMethod 
			float pmid = (minValue + maxValue) / 2;
			BVHPrimitiveInfo *midPtr =
				std::partition(&primitiveInfo[start], &primitiveInfo[end - 1] + 1,
					[dim, pmid](const BVHPrimitiveInfo &pi) {
				switch (dim) {
				case 0:
					return pi.centroid.x < pmid;
					break;
				case 1:
					return pi.centroid.y < pmid;
					break;
				case 2:
					return pi.centroid.z < pmid;
					break;
				default:
					break;
				}
			});
			mid = midPtr - &primitiveInfo[0];

			{
				node->InitInterior(dim,
					recursiveBuild(primitiveInfo, start, mid,
						totalNodes, orderedPrims),
					recursiveBuild(primitiveInfo, mid, end,
						totalNodes, orderedPrims));
			}
		}

	}
	return node;
}

int BVH::MaximumExtent(const BBox centerBounds) {
	Vector d = centerBounds.diagonal();
	if (d.x > d.y && d.x > d.z)
		return 0;
	else if (d.y > d.z)
		return 1;
	else
		return 2;
}

BBox BVH::getBounds() const {
	BBox fullGroupBox;
	fullGroupBox = fullGroupBox.empty();
	for (Primitive* p : primitives) {
		BBox primBox = p->getBounds();
		fullGroupBox.extend(primBox);
	}
	return fullGroupBox;
}

Intersection BVH::intersect(const Ray& ray, float previousBestDistance) const {
	Intersection intrsn = Intersection::failure();
	float shortestDistance = previousBestDistance;
	bool hit = false;
	Vector invDir(1 / ray.d.x, 1 / ray.d.y, 1 / ray.d.z);
	int dirIsNeg[3] = { invDir.x < 0, invDir.y < 0, invDir.z < 0 };

	int toVisitOffset = 0, currentNodeIndex = 0;
	static std::stack<int> nodesToVisit;
	while (true) {
		const LinearBVHNode *node = &nodes[currentNodeIndex];
		// Check ray against BVH node 
		std::pair<float, float> bboxInter;
		bboxInter = node->bounds.intersect(ray);
	//if (!(bboxInter.first==1 && bboxInter.second ==0 )) {
			if (bboxInter.first<= bboxInter.second) {
				if (node->nPrimitives > 0) {
					// Intersect ray with primitives in leaf BVH node 
					
					for (int i = 0; i < node->nPrimitives; ++i) {
						Intersection intersectTemp = primitives[node->primitivesOffset + i]->intersect(ray, shortestDistance);
						if (intersectTemp && intersectTemp.distance < shortestDistance) {
							shortestDistance = intersectTemp.distance;
							intrsn = intersectTemp;
						}
					}
					if (nodesToVisit.size() == 0) break;
					currentNodeIndex = nodesToVisit.top();
					nodesToVisit.pop();
				}
				else {
					// Put far BVH node on nodesToVisit stack, advance to near node 
					if (dirIsNeg[node->axis]) {
						nodesToVisit.push(currentNodeIndex + 1);
						currentNodeIndex = node->secondChildOffset;
					}
					else {
						nodesToVisit.push(node->secondChildOffset);
						currentNodeIndex = currentNodeIndex + 1;
					}
				}
			}
			else {
				if (nodesToVisit.size() == 0) break;
				currentNodeIndex = nodesToVisit.top();
				nodesToVisit.pop();
			}

	}
	return intrsn;
}

void BVH::add(Primitive* p) {
	this->primitives.push_back(p);
}

void BVH::setMaterial(Material* m) {
	for (Primitive* p : primitives) {
		p->setMaterial(m);
	}
}

void BVH::setCoordMapper(CoordMapper* cm) {
	for (Primitive* p : primitives) {
		p->setCoordMapper(cm);
	}
}

void BVH::serialize(BVH::Output& output) {
    // To implement this function:
    // - Call output.setNodeCount() with the number of nodes in the BVH
    /* TODO */
    // - Set the root node index using output.setRootId()
    /* TODO */
    // - Write each and every one of the BVH nodes to the output using output.writeNode()
    /* TODO */ NOT_IMPLEMENTED;
}
void BVH::deserialize(BVH::Input& input) {
    // To implement this function:
    // - Allocate and initialize input.getNodeCount() nodes
    /* TODO */
    // - Fill your nodes with input.readNode(index)
    /* TODO */
    // - Use the node at index input.getRootId() as the root node
    /* TODO */ NOT_IMPLEMENTED;
}

}