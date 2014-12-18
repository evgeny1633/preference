#ifndef CLIENT_COMMAND_SENDER_H
#define CLIENT_COMMAND_SENDER_H

#include <QDialog>

namespace Ui {
class client_command_sender;
}

class client_command_sender : public QDialog
{
    Q_OBJECT

public:
    explicit client_command_sender(QWidget *parent = 0);
    ~client_command_sender();

    QString text_block_2;
    QString text_block_3;

    void connect(boost::asio::ip::tcp::socket socket);

signals:
    void send_message();

private slots:
    void on_pushButton_clicked();

    void on_block_2_textChanged(const QString &arg1);

    void on_block_3_textChanged(const QString &arg1);


private:
    Ui::client_command_sender *ui;
    boost::asio::ip::tcp::socket *socket;
};

#endif // CLIENT_COMMAND_SENDER_H
