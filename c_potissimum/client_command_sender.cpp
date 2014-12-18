#include "client_command_sender.h"
#include "ui_client_command_sender.h"
#include "../include/include.h"

client_command_sender::client_command_sender(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::client_command_sender)
{
    ui->setupUi(this);
}

client_command_sender::~client_command_sender()
{
    delete ui;
}

void client_command_sender::connect(boost::asio::ip::tcp::socket socket)
{
    boost::asio::ip::tcp::socket* socket = new boost::asio::ip::tcp::socket(std::move(sock));
    this->socket = socket;
}

void client_command_sender::send_message()
{


}

void client_command_sender::on_pushButton_clicked()
{

}

void client_command_sender::on_block_2_textChanged(const QString &arg1)
{
    text_block_2 = arg1;
}

void client_command_sender::on_block_3_textChanged(const QString &arg1)
{
    text_block_3 = arg1;
}
