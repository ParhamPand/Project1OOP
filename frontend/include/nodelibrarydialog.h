#ifndef NODELIBRARYDIALOG_H
#define NODELIBRARYDIALOG_H

#include <QDialog>
#include <string>

// Forward declare this class to avoid including the full header
class QTreeWidgetItem;

namespace Ui {
    class NodeLibraryDialog;
}

class NodeLibraryDialog : public QDialog
{
Q_OBJECT

public:
    explicit NodeLibraryDialog(QWidget *parent = nullptr);
    ~NodeLibraryDialog();

    // Public function to get the result after the dialog closes
    std::string getSelectedComponent() const;

private slots:
    // A slot to handle when the user double-clicks an item in the tree
    void on_componentTreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_searchLineEdit_textChanged(const QString &text);

private:
    Ui::NodeLibraryDialog *ui;
    // Private variable to store the selected component's name
    std::string m_selectedComponent;
};

#endif // NODELIBRARYDIALOG_H