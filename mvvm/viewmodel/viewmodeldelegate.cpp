// ************************************************************************** //
//
//  Prototype of mini MVVM framework for bornagainproject.org
//
//! @homepage  http://www.bornagainproject.org
//! @license   GNU General Public License v3 or higher
//
// ************************************************************************** //

#include "viewmodeldelegate.h"
#include "comboproperty.h"
#include "customeditor.h"
#include "editorfactory.h"
#include <QApplication>
#include <QDebug>

using namespace ModelView;

ViewModelDelegate::ViewModelDelegate(QObject* parent)
    : QStyledItemDelegate(parent), m_editor_factory(std::make_unique<EditorFactory>())
{
}

void ViewModelDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
                              const QModelIndex& index) const
{
    if (m_editor_factory->hasStringRepresentation(index)) {
        QString text = QString::fromStdString(m_editor_factory->toString(index));
        paintCustomLabel(painter, option, index, text);
    } else {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

ViewModelDelegate::~ViewModelDelegate() = default;

QWidget* ViewModelDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option,
                                         const QModelIndex& index) const
{
    qDebug() << "ViewModelDelegate::createEditor" << parent << option << index;

    if (auto editor = m_editor_factory->createEditor(index, parent)) {
        connect(editor, &CustomEditor::dataChanged, this,
                &ViewModelDelegate::onCustomEditorDataChanged);
        return editor;
    } else {
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
}

void ViewModelDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    if (!index.isValid())
        return;

    qDebug() << "ViewModelDelegate::setEditorData" << editor << index;

    if (auto customEditor = dynamic_cast<CustomEditor*>(editor))
        customEditor->setData(index.data());
    else
        QStyledItemDelegate::setEditorData(editor, index);
}

void ViewModelDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
                                     const QModelIndex& index) const
{
    if (!index.isValid())
        return;

    qDebug() << "ViewModelDelegate::setModelData" << editor << model << index;

    if (auto customEditor = dynamic_cast<CustomEditor*>(editor)) {
        model->setData(index, customEditor->data());
    } else {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

void ViewModelDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option,
                                             const QModelIndex& index) const
{
    qDebug() << "ViewModelDelegate::updateEditorGeometry" << editor << option << index;
    QStyledItemDelegate::updateEditorGeometry(editor, option, index);
}

void ViewModelDelegate::onCustomEditorDataChanged()
{
    qDebug() << "ViewModelDelegate::onCustomEditorDataChanged";
    CustomEditor* editor = qobject_cast<CustomEditor*>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}

void ViewModelDelegate::paintCustomLabel(QPainter* painter, const QStyleOptionViewItem& option,
                                         const QModelIndex& index, const QString& text) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index); // calling original method to take into accounts colors etc
    opt.text = displayText(text, option.locale); // by overriding text with ours
    const QWidget* widget = opt.widget;
    QStyle* style = widget ? widget->style() : QApplication::style();
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);
}