// ************************************************************************** //
//
//  Prototype of mini MVVM framework for bornagainproject.org
//
//! @homepage  http://www.bornagainproject.org
//! @license   GNU General Public License v3 or higher
//
// ************************************************************************** //

#include "externalpropertyeditor.h"
#include "customeventfilters.h"
#include "customvariants.h"
#include "externalproperty.h"
#include "styleutils.h"
#include <QColor>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QMessageBox>

using namespace ModelView;

ExternalPropertyEditor::ExternalPropertyEditor(QWidget* parent)
    : CustomEditor(parent), m_textLabel(new QLabel), m_pixmapLabel(new QLabel),
      m_focusFilter(new LostFocusFilter(this))

{
    setMouseTracking(true);
    setAutoFillBackground(true);

    auto layout = new QHBoxLayout;
    layout->setContentsMargins(4, 0, 0, 0);

    auto button = new QToolButton;
    button->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred));
    button->setText(QLatin1String(" . . . "));
    button->setToolTip("Open selector");
    layout->addWidget(m_pixmapLabel);
    layout->addWidget(m_textLabel);
    layout->addStretch(1);
    layout->addWidget(button);
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_InputMethodEnabled);
    connect(button, &QToolButton::clicked, this, &ExternalPropertyEditor::buttonClicked);

    setLayout(layout);
}

void ExternalPropertyEditor::buttonClicked()
{
    if (m_callback)
        m_callback(m_data);
    else
        QMessageBox::warning(nullptr, "Not configured", "No external dialog configured");
}

void ExternalPropertyEditor::update_components()
{
    if (!Utils::IsExtPropertyVariant(m_data))
        throw std::runtime_error("Error. Wrong variant type (ExternalProperty is required).");

    ExternalProperty prop = m_data.value<ExternalProperty>();
    QPixmap pixmap(Style::DefaultPixmapSize(), Style::DefaultPixmapSize());
    pixmap.fill(prop.color());
    m_textLabel->setText(QString::fromStdString(prop.text()));
    m_pixmapLabel->setPixmap(pixmap);
}