#include "nodelibrarydialog.h"
#include "ui_nodelibrarydialog.h"
#include <QTreeWidgetItem>

NodeLibraryDialog::NodeLibraryDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::NodeLibraryDialog)
{
    ui->setupUi(this);
    setWindowTitle("Component Library");

    // --- Populate the Tree Widget ---
    ui->componentTreeWidget->setHeaderLabels({"Component Name"});

    QTreeWidgetItem *passives = new QTreeWidgetItem(ui->componentTreeWidget);
    passives->setText(0, "Passive Components");

    (new QTreeWidgetItem(passives))->setText(0, "Resistor");
    (new QTreeWidgetItem(passives))->setText(0, "Capacitor");
    (new QTreeWidgetItem(passives))->setText(0, "Inductor");

    QTreeWidgetItem *sources = new QTreeWidgetItem(ui->componentTreeWidget);
    sources->setText(0, "Sources");

    (new QTreeWidgetItem(sources))->setText(0, "Voltage Source");
    (new QTreeWidgetItem(sources))->setText(0, "Current Source");

    // Expand all categories by default
    ui->componentTreeWidget->expandAll();
}

NodeLibraryDialog::~NodeLibraryDialog()
{
    delete ui;
}

// Implementation for our public getter function
std::string NodeLibraryDialog::getSelectedComponent() const
{
    return m_selectedComponent;
}

// This slot is automatically connected by Qt to the tree widget's double-click signal
// because of its name: on_<objectName>_<signalName>
void NodeLibraryDialog::on_componentTreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    // Check if the clicked item is a child (a component) and not a category
    if (item->childCount() == 0) {
        // Store the component's name
        m_selectedComponent = item->text(0).toStdString();
        // Accept and close the dialog
        accept();
    }
}

void NodeLibraryDialog::on_searchLineEdit_textChanged(const QString &text)
{
    // Create an iterator to go through all items in the tree
    QTreeWidgetItemIterator it(ui->componentTreeWidget);

    while (*it) {
        QTreeWidgetItem *item = *it;
        // Check if the item's text contains the search text (case-insensitive)
        bool matches = item->text(0).contains(text, Qt::CaseInsensitive);

        // Hide or show the item based on the match
        item->setHidden(!matches);

        // --- Important: Keep parents visible ---
        // If a child item matches, we must make sure its parent category is also visible.
        if (matches && item->parent()) {
            item->parent()->setHidden(false);
        }

        ++it;
    }
}