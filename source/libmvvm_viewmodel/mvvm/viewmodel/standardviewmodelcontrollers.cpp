// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#include <mvvm/model/groupitem.h>
#include <mvvm/viewmodel/labeldatarowstrategy.h>
#include <mvvm/viewmodel/propertiesrowstrategy.h>
#include <mvvm/viewmodel/standardviewitems.h>
#include <mvvm/viewmodel/standardchildrenstrategies.h>
#include <mvvm/viewmodel/standardviewmodelcontrollers.h>

using namespace ModelView;

// ----------------------------------------------------------------------------

DefaultViewModelController::DefaultViewModelController(SessionModel* session_model,
                                                       ViewModelBase* view_model)
    : ViewModelController(session_model, view_model)
{
    setRowStrategy(std::make_unique<LabelDataRowStrategy>());
    setChildrenStrategy(std::make_unique<AllChildrenStrategy>());
}

// ----------------------------------------------------------------------------

TopItemsViewModelController::TopItemsViewModelController(SessionModel* session_model,
                                                         ViewModelBase* view_model)
    : ViewModelController(session_model, view_model)
{
    setRowStrategy(std::make_unique<LabelDataRowStrategy>());
    setChildrenStrategy(std::make_unique<TopItemsStrategy>());
}

// ----------------------------------------------------------------------------

PropertyViewModelController::PropertyViewModelController(SessionModel* session_model,
                                                         ViewModelBase* view_model)
    : ViewModelController(session_model, view_model)
{
    setRowStrategy(std::make_unique<LabelDataRowStrategy>());
    setChildrenStrategy(std::make_unique<PropertyItemsStrategy>());
}

void PropertyViewModelController::onDataChange(SessionItem* item, int role)
{
    ViewModelController::onDataChange(item, role);
    // If data change occured with GroupItem, performs cleanup and regeneration of
    // ViewItems, corresponding to groupItem's current index.
    if (auto group = dynamic_cast<GroupItem*>(item))
        update_branch(group);
}

// ----------------------------------------------------------------------------

PropertyTableViewModelController::PropertyTableViewModelController(
    SessionModel* session_model, ViewModelBase* view_model, const std::vector<std::string>& labels)
    : ViewModelController(session_model, view_model)
{
    setRowStrategy(std::make_unique<PropertiesRowStrategy>(labels));
    setChildrenStrategy(std::make_unique<TopItemsStrategy>());
}

// ----------------------------------------------------------------------------

PropertyFlatViewModelController::PropertyFlatViewModelController(SessionModel* session_model,
                                                                 ViewModelBase* view_model)
    : ViewModelController(session_model, view_model)
{
    setRowStrategy(std::make_unique<LabelDataRowStrategy>());
    setChildrenStrategy(std::make_unique<PropertyItemsFlatStrategy>());
}

void PropertyFlatViewModelController::onDataChange(SessionItem* item, int role)
{
    ViewModelController::onDataChange(item, role);
    // If data change occured with GroupItem, performs cleanup and regeneration of
    // ViewItems, corresponding to groupItem's current index.
    if (auto group = dynamic_cast<GroupItem*>(item))
        update_branch(group->parent());
}
