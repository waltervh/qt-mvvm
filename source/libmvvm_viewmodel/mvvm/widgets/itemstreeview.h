// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#ifndef MVVM_WIDGETS_ITEMSTREEVIEW_H
#define MVVM_WIDGETS_ITEMSTREEVIEW_H

#include <QWidget>
#include <memory>
#include <mvvm/core/export.h>

class QTreeView;
class QItemSelection;
class QItemSelectionModel;

namespace ModelView
{

class SessionItem;
class ViewModel;
class ViewModelDelegate;

//! Tree view to show items of SessionModel via ViewModel mechanism.
//!
//! Provides notification mechanism for SessionItem selections, use custom delegate.

class CORE_EXPORT ItemsTreeView : public QWidget
{
    Q_OBJECT

public:
    explicit ItemsTreeView(QWidget* parent = nullptr);
    ~ItemsTreeView() override;

    QTreeView* treeView();

    void setViewModel(std::unique_ptr<ViewModel> viewModel);

    void setSelected(SessionItem* item);

    void setRootSessionItem(SessionItem* item);

    ViewModel* viewModel() const;

signals:
    void itemSelected(ModelView::SessionItem*);

private slots:
    void onSelectionChanged(const QItemSelection&, const QItemSelection&);

private:
    QItemSelectionModel* selectionModel();

    void set_connected(bool flag);

    QTreeView* m_treeView;
    std::unique_ptr<ViewModel> m_viewModel;
    std::unique_ptr<ViewModelDelegate> m_delegate;
    bool m_block_selection;
};

} // namespace ModelView

#endif // MVVM_WIDGETS_ITEMSTREEVIEW_H
