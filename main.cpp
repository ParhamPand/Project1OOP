#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    // 1. یک نمونه از کل اپلیکیشن Qt می‌سازیم.
    QApplication a(argc, argv);

    // 2. یک نمونه از پنجره اصلی که خودمان طراحی می‌کنیم، می‌سازیم.
    MainWindow w;

    // 3. پنجره را نمایش می‌دهیم.
    w.show();

    // 4. اپلیکیشن را وارد حلقه رویدادها (Event Loop) می‌کنیم تا منتظر تعامل کاربر بماند.
    return a.exec();
}