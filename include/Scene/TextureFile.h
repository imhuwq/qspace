#ifndef QSPACE_TEXTUREFILE_H
#define QSPACE_TEXTUREFILE_H

#include <QString>

class TextureFile {
public:
    explicit TextureFile(const QString &texture_path) : m_path(texture_path) {}

    QString path() { return m_path; }

private:
    QString m_path;
};

#endif //QSPACE_TEXTUREFILE_H
