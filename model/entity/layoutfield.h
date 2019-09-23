#ifndef LAYOUTFIELD_H
#define LAYOUTFIELD_H

#include <QObject>

#include "layout.h"

class DBTable;

class LayoutField : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(int layoutId READ layoutId WRITE setLayoutId)
    Q_PROPERTY(int type READ type WRITE setType)
    Q_PROPERTY(QString font READ font WRITE setFont)
    Q_PROPERTY(double x READ x WRITE setX)
    Q_PROPERTY(double y READ y WRITE setY)
    Q_PROPERTY(double width READ width WRITE setWidth)
    Q_PROPERTY(double height READ height WRITE setHeight)
    Q_PROPERTY(QString value READ value WRITE setValue)
    Q_PROPERTY(int align READ align WRITE setAlign)
    Q_PROPERTY(int layer READ layer WRITE setLayer)

    Q_PROPERTY(Layout *layout READ layout WRITE setLayout STORED false)

public:
    Q_INVOKABLE LayoutField() = default;

    static const DBTable *mapping();

    int id() const;
    void setId(int id);

    int layoutId() const;
    void setLayoutId(int layoutId);

    Layout *layout() const;
    void setLayout(Layout *layout);

    int type() const;
    void setType(int type);

    QString font() const;
    void setFont(const QString &font);

    double x() const;
    void setX(double x);

    double y() const;
    void setY(double y);

    double width() const;
    void setWidth(double width);

    double height() const;
    void setHeight(double height);

    QString value() const;
    void setValue(const QString &value);

    int align() const;
    void setAlign(int align);

    int layer() const;
    void setLayer(int layer);

private:
    int m_id = 0;
    int m_layoutId;
    Layout *m_layout;
    int m_type;
    QString m_font;
    double m_x;
    double m_y;
    double m_width;
    double m_height;
    QString m_value;
    int m_align;
    int m_layer;

    static DBTable *initializeMapping();
    static const DBTable *m_mapping;
};
Q_DECLARE_METATYPE(LayoutField *);

#endif // LAYOUTFIELD_H
