#include "doublespinboxdelegate.h"
#include <QDoubleSpinBox>

QWidget *DoubleSpinBoxDelegate::createEditor(QWidget *parent,
                                             const QStyleOptionViewItem &,
                                             const QModelIndex &) const
{
    QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
    return editor;
}

void DoubleSpinBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();
    QDoubleSpinBox *sbx = static_cast<QDoubleSpinBox *>(editor);
    sbx->setValue(value.toDouble());
}

void DoubleSpinBoxDelegate::setModelData(QWidget *editor,
                                         QAbstractItemModel *model,
                                         const QModelIndex &index) const
{
    QDoubleSpinBox *sbx = static_cast<QDoubleSpinBox *>(editor);
    QString value = QString::number(sbx->value());
    model->setData(index, value, Qt::EditRole);
}

void DoubleSpinBoxDelegate::updateEditorGeometry(QWidget *editor,
                                                 const QStyleOptionViewItem &option,
                                                 const QModelIndex &) const
{
    editor->setGeometry(option.rect);
}
