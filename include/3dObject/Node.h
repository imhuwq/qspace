#ifndef QSPACE_NODE_H
#define QSPACE_NODE_H

#include <QMap>
#include <QString>
#include <QVector>
#include <QMatrix4x4>
#include <QSharedPointer>


using namespace std;

class Node;

typedef QSharedPointer<Node> NodePtr;

typedef QSharedPointer<const Node> kNodePtr;

enum NodeType {
    kGeneral = 0,
    kCamera,
    kMesh,
    kNodeTypeCount
};

class Node {
public:
    explicit Node(QString name, NodeType type = NodeType::kGeneral) : name_(move(name)), type_(type), translation_(0, 0, 0), rotation_(0, 0, 0), scale_(1, 1, 1), dirty_(true) {
        ComputeTransform();
    }

    void SetName(const QString &name) { name_ = name; }

    QString GetName() const { return name_; }

    void Translate(const QVector3D &translation) {
        dirty_ = true;
        translation_ += translation;
    }

    void SetTranslation(const QVector3D &translation) {
        dirty_ = true;
        translation_ = translation;
    }

    QVector3D GetTranslation() const { return translation_; }

    void Rotate(const QVector3D &rotation) {
        dirty_ = true;
        rotation_ = QQuaternion::fromEulerAngles(rotation).rotatedVector(rotation_);
    }

    void SetRotation(const QVector3D &rotation) {
        dirty_ = true;
        rotation_ = rotation;
    }

    QVector3D GetRotation() const { return rotation_; }

    void Scale(const QVector3D &scale) {
        dirty_ = true;
        scale_ *= scale;
    }

    void Scale(double ratio) {
        dirty_ = true;
        scale_ *= ratio;
    }

    void SetScale(const QVector3D &scale) {
        dirty_ = true;
        scale_ = scale;
    }

    QVector3D GetScale() const { return scale_; }

    QMatrix4x4 GetTransformation() const { return transformation_; }

    QVector<kNodePtr> GetNodes() const {
        QVector<kNodePtr> nodes;
        for (auto &node_ptr:nodes_) nodes.push_back(node_ptr);
        return nodes;
    }

    void AddNode(const NodePtr &node) { nodes_.push_back(node); }

    NodeType GetType() const { return type_; }

    void ComputeTransform() {
        if (dirty_) {
            dirty_ = false;
            transformation_.setToIdentity();
            transformation_.translate(translation_);
            QQuaternion quaternion = QQuaternion::fromEulerAngles(rotation_);
            transformation_.rotate(quaternion);
            transformation_.scale(scale_);
        }
    }

    virtual ~Node() = default;

protected:
    QString name_;
    NodeType type_;
    bool dirty_ = false;
    QVector3D translation_;
    QVector3D rotation_;
    QVector3D scale_;
    QMatrix4x4 transformation_;
    QVector<NodePtr> nodes_;
};

#endif //QSPACE_NODE_H
