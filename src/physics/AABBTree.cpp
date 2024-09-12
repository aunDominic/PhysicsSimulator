#include <physics/AABBTree.hpp>

AABBTreeNode::AABBTreeNode() : left(nullptr), right(nullptr) {}

bool AABBTreeNode::isLeaf() const {
    return left == nullptr && right == nullptr;
}

AABBTree::AABBTree() : root(new AABBTreeNode()) {}

AABBTree::~AABBTree() {
    deleteNode(root);
}

void AABBTree::deleteNode(AABBTreeNode* node) {
    if (node) {
        deleteNode(node->left);
        deleteNode(node->right);
        delete node;
    }
}

void AABBTree::insert(const AABB& object) {
    objects.push_back(object);
    insertObject(objects.size() - 1, root);
}

void AABBTree::insertObject(int objectIndex, AABBTreeNode* node) {
    if (node->isLeaf()) {
        if (node->objectIndices.size() < 4) { // Adjust this threshold as needed
            node->objectIndices.push_back(objectIndex);
            node->boundingBox.merge(objects[objectIndex]);
        } else {
            // Split the node
            node->left = new AABBTreeNode();
            node->right = new AABBTreeNode();
            
            // Distribute objects between left and right nodes
            for (int index : node->objectIndices) {
                if (objects[index].getCenter().x < node->boundingBox.getCenter().x) {
                    node->left->objectIndices.push_back(index);
                    node->left->boundingBox.merge(objects[index]);
                } else {
                    node->right->objectIndices.push_back(index);
                    node->right->boundingBox.merge(objects[index]);
                }
            }
            
            // Insert the new object
            insertObject(objectIndex, node->left->boundingBox.contains(objects[objectIndex].getCenter()) ? node->left : node->right);
            
            node->objectIndices.clear();
        }
    } else {
        // Insert into the child that requires the least expansion
        AABB leftExpanded = node->left->boundingBox;
        AABB rightExpanded = node->right->boundingBox;
        leftExpanded.merge(objects[objectIndex]);
        rightExpanded.merge(objects[objectIndex]);
        
        float leftExpansion = glm::length(leftExpanded.getDimensions() - node->left->boundingBox.getDimensions());
        float rightExpansion = glm::length(rightExpanded.getDimensions() - node->right->boundingBox.getDimensions());
        
        if (leftExpansion < rightExpansion) {
            insertObject(objectIndex, node->left);
        } else {
            insertObject(objectIndex, node->right);
        }
        
        node->boundingBox.merge(objects[objectIndex]);
    }
}

std::vector<int> AABBTree::query(const AABB& queryBox) const {
    std::vector<int> result;
    queryNode(queryBox, root, result);
    return result;
}

void AABBTree::queryNode(const AABB& queryBox, const AABBTreeNode* node, std::vector<int>& result) const {
    if (node->boundingBox.intersects(queryBox)) {
        if (node->isLeaf()) {
            for (int index : node->objectIndices) {
                if (objects[index].intersects(queryBox)) {
                    result.push_back(index);
                }
            }
        } else {
            queryNode(queryBox, node->left, result);
            queryNode(queryBox, node->right, result);
        }
    }
}