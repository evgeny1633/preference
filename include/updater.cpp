#include "updater.h"

// Updater::Updater()
// {
// }
void Updater::send_message_slot(QString message)
{
    emit send_message_signal(message);
    std::cout << "Updater::send_message_slot. message:" << message.toUtf8().constData() << std::endl;
}