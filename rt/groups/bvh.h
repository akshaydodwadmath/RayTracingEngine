#ifndef CG1RAYTRACER_GROUPS_BVH_HEADER
#define CG1RAYTRACER_GROUPS_BVH_HEADER

#include <rt/groups/group.h>
#include <rt/bbox.h>

namespace rt {
	struct BVHPrimitiveInfo {
		int primitiveNumber;
		BBox bounds;
		Point centroid;
	};

	struct BVHBuildNode {
		void InitInterior(int axis, BVHBuildNode *c0, BVHBuildNode *c1) {
			children[0] = c0;
			children[1] = c1;
			bounds = c0->bounds;
			bounds.extend(c1->bounds);
			splitAxis = axis;
			nPrimitives = 0;
		}

		void InitLeaf(int first, int n, const BBox &b) {
			firstPrimOffset = first;
			nPrimitives = n;
			bounds = b;
			children[0] = children[1] = nullptr;
		}
		BBox bounds; //bounds of all children
		BVHBuildNode *children[2]; //Pointer to childrens
		int splitAxis;
		int firstPrimOffset; //which primitive is stored(leaf node)
		int nPrimitives;
	};

	struct LinearBVHNode {
		BBox bounds;
		union {
			int primitivesOffset;    // leaf
			int secondChildOffset;   // interior
		};
		uint16_t nPrimitives;  // 0 -> interior node
		uint8_t axis;          // interior node: xyz
		uint8_t pad[1];        // ensure 32 byte total size
	};

	class BVH : public Group {
	public:
		BVH();

		virtual BBox getBounds() const;
		virtual Intersection intersect(const Ray& ray, float previousBestDistance = FLT_MAX) const;
		virtual void rebuildIndex();
		virtual void add(Primitive* p);
		virtual void setMaterial(Material* m);
		virtual void setCoordMapper(CoordMapper* cm);
		// Do not use this structure as your node layout:
		// It is inefficient and has a large memory footprint.
		struct SerializedNode {
			bool isLeaf;
			BBox bbox;

			// For leaves only
			std::vector<Primitive*> primitives;

			// For internal nodes only
			size_t leftChildId;
			size_t rightChildId;
		};

		// Implement this function if you want to take part in the BVH speed competition
		struct Output {
			virtual void setNodeCount(size_t nodeCount) = 0;
			virtual void setRootId(size_t rootId) = 0;
			virtual void writeNode(size_t nodeId, const SerializedNode& node) = 0;
		};
		void serialize(Output& output);

		// Implementing this is optional and not needed for the BVH speed competition,
		// but it will allow you to run the benchmark utility and test your BVH exporter.
		struct Input {
			virtual size_t getNodeCount() = 0;
			virtual size_t getRootId() = 0;
			virtual const SerializedNode& readNode(size_t nodeId) = 0;
		};
		void deserialize(Input& input);

		void storePrimitiveInfo(std::vector<BVHPrimitiveInfo> &primitiveInfo);
		BVHBuildNode*  recursiveBuild(std::vector<BVHPrimitiveInfo> &primitiveInfo, int start, int end,
			int *totalNodes, Primitives &orderedPrims);

		int primNumi = 0;

		int MaximumExtent(const BBox centerBounds);
		LinearBVHNode *nodes = nullptr;

		int createLinearTree(BVHBuildNode *node, int *offset);
	};





	/*int MaximumExtent(const BBox centerBounds) {
		Vector d = centerBounds.diagonal();
		if (d.x > d.y && d.x > d.z)
			return 0;
		else if (d.y > d.z)
			return 1;
		else
			return 2;
	}*/


}
#endif