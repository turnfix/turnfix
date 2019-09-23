#include "layoutfield.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"

DBTable *LayoutField::initializeMapping()
{
    DBTable *layoutField = new DBTable("tfx_layout_felder");
    layoutField->addColumn("id", "int_layout_felderid", ColumnType::Integer, 0, false, "", "", true);
    layoutField->addColumn("layoutId", "int_layoutid", ColumnType::Integer, 0, false)
        ->addContraint("fky_layoutid", "tfx_layouts", "int_layoutid", "RESTRICT", "CASCADE");
    layoutField->addColumn("type", "int_typ", ColumnType::SmallInt);
    layoutField->addColumn("font", "var_font", ColumnType::Varchar, 150);
    layoutField->addColumn("x", "rel_x", ColumnType::Real);
    layoutField->addColumn("y", "rel_y", ColumnType::Real);
    layoutField->addColumn("width", "rel_w", ColumnType::Real);
    layoutField->addColumn("height", "rel_h", ColumnType::Real);
    layoutField->addColumn("value", "var_value", ColumnType::Varchar, 200);
    layoutField->addColumn("align", "int_align", ColumnType::SmallInt, 0, true, "0");
    layoutField->addColumn("layer", "int_layer", ColumnType::SmallInt);

    return layoutField;
}

const DBTable *LayoutField::m_mapping = LayoutField::initializeMapping();

const DBTable *LayoutField::mapping()
{
    return m_mapping;
}

int LayoutField::id() const
{
    return m_id;
}

void LayoutField::setId(int id)
{
    m_id = id;
}

int LayoutField::layoutId() const
{
    return m_layoutId;
}

void LayoutField::setLayoutId(int layoutId)
{
    m_layoutId = layoutId;
}

Layout *LayoutField::layout() const
{
    return m_layout;
}

void LayoutField::setLayout(Layout *layout)
{
    m_layout = layout;
    if (layout == nullptr) {
        m_layoutId = 0;
        return;
    }

    m_layoutId = layout->id();
}

int LayoutField::type() const
{
    return m_type;
}

void LayoutField::setType(int type)
{
    m_type = type;
}

QString LayoutField::font() const
{
    return m_font;
}

void LayoutField::setFont(const QString &font)
{
    m_font = font;
}

double LayoutField::x() const
{
    return m_x;
}

void LayoutField::setX(double x)
{
    m_x = x;
}

double LayoutField::y() const
{
    return m_y;
}

void LayoutField::setY(double y)
{
    m_y = y;
}

double LayoutField::width() const
{
    return m_width;
}

void LayoutField::setWidth(double width)
{
    m_width = width;
}

double LayoutField::height() const
{
    return m_height;
}

void LayoutField::setHeight(double height)
{
    m_height = height;
}

QString LayoutField::value() const
{
    return m_value;
}

void LayoutField::setValue(const QString &value)
{
    m_value = value;
}

int LayoutField::align() const
{
    return m_align;
}

void LayoutField::setAlign(int align)
{
    m_align = align;
}

int LayoutField::layer() const
{
    return m_layer;
}

void LayoutField::setLayer(int layer)
{
    m_layer = layer;
}
