#ifndef QSPACE_MATERIAL_H
#define QSPACE_MATERIAL_H

#include <QMap>
#include <QString>
#include <QVector3D>
#include <QVector4D>

class Material {
public:
    const QString &name() const { return m_name; }

    void setName(const QString &name) { m_name = name; }

    const QVector3D &ambient() const { return m_ambient; }

    void setAmbient(const QVector3D &ambient) { m_ambient = ambient; }

    const QVector3D &diffuse() const { return m_diffuse; }

    void setDiffuse(const QVector3D &diffuse) { m_diffuse = diffuse; }

    const QVector3D &specular() const { return m_specular; }

    void setSpecular(const QVector3D &specular) { m_specular = specular; }

    float shininess() const { return m_shininess; }

    void setShininess(float shininess) { m_shininess = shininess; }

    QString diffusePath() { return m_diffusePath; }

    void setDiffusePath(const QString &path) { m_diffusePath = path; }

    QMap<QString, QString> texturePaths();

private:
    QString m_name;
    QVector3D m_ambient;
    QVector3D m_diffuse;
    QVector3D m_specular;
    float m_shininess;

    QString m_diffusePath;
};

#endif //QSPACE_MATERIAL_H
