#include "updater.h"

// Updater::Updater()
// {
// }
void Updater::send_message_slot(QString new_message)
{
    emit send_message_signal(new_message);
    std::cout << "Updater::send_message_slot. message:" << new_message.toUtf8().constData() << std::endl;
//     qApp->processEvents();
}

// void Updater::send_message_signal(QString message)
// {
//     std::cout << "Updater::send_message_signal. message:" << message.toUtf8().constData() << std::endl;
//     qApp->processEvents();
// }