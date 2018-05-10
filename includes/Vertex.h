#ifndef QSPACE_VERTEX_H
#define QSPACE_VERTEX_H

#include <QVector3D>

class Vertex {
public:
    Q_DECL_CONSTEXPR Vertex() {}

    Q_DECL_CONSTEXPR explicit Vertex(const QVector3D &position) : m_position(position) {}

    Q_DECL_CONSTEXPR Vertex(const QVector3D &position, const QVector3D &color) : m_position(position), m_color(color) {}

    Q_DECL_CONSTEXPR const QVector3D &position() const { return m_position; }

    Q_DECL_CONSTEXPR const QVector3D &color() const { return m_color; };

    void setPosition(const QVector3D &position) { m_position = position; }

    void setColor(const QVector3D &color) { m_color = color; }

    static const int PositionTupleSize = 3;
    static const int ColorTupleSize = 3;

    static Q_DECL_CONSTEXPR int positionOffset() { return offsetof(Vertex, m_position); }

    static Q_DECL_CONSTEXPR int colorOffset() { return offsetof(Vertex, m_color); }

    static Q_DECL_CONSTEXPR int stride() { return sizeof(Vertex); }

private:
    QVector3D m_position;
    QVector3D m_color;
};

// 可以使用 memcpy 进行拷贝
Q_DECLARE_TYPEINFO(Vertex, Q_MOVABLE_TYPE);

#endif //QSPACE_VERTEX_H
