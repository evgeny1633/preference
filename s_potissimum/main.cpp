#include <iostream>
#include <sstream>
#include <unistd.h>
#include "widget.h"
#include "log.h"
#include "../include/include.h"
#include <QThread>
#include <QApplication>

using namespace std;

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  Widget w;
  //w.show();
  std::stringstream ss;
//   Log log;
//   QTextEdit *textBox = log.findChild<QTextEdit *>("textEdit");  
//   log.show();
//   for(int i = 0; i < 10; i++  )
//   {
//       ss.str("");
//       ss << i;
//       textBox->append(QString::fromStdString(ss.str()));
//   }
//   log.show();
//   sleep(2);
//   for(int i = 11; i < 20; i++)
//   {
//       ss.str("");
//       ss << i;
//       textBox->append(QString::fromStdString(ss.str()));
//       log.repaint();
// //       sleep(1);
//   }

  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: ./_server <port>\n";
      return 1;
    }

    boost::asio::io_service io_service;
    //start the server
    server(io_service, std::atoi(argv[1]));
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }
  
  return a.exec();
}


using boost::asio::ip::tcp;
void server(boost::asio::io_service& io_service, unsigned short port)
{
  std::cout << "++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
  std::cout << "Server is started " << std::endl;
  std::cout << "++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
  std::string message;
  std::stringstream ss;
  Log log;
  QTextEdit *textBox = log.findChild<QTextEdit *>("textEdit");
//   QThread* logthread;
  log.show();
//   QObject::connect(textBox,SIGNAL(valueChanged(int)), logthread, SLOT(setNum(int)));
//   textBox->moveToThread(logthread);
//   logthread->start();
  
  for(int i = 0; i < 10; i++  )
  {
      ss.str("");
      ss << i;
      textBox->append(QString::fromStdString(ss.str()));
  }
//  log.show();
//   sleep(2);
//      log.repaint();

  tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));
  for (;;)
  {
    tcp::socket sock(io_service);    //open socket
    acceptor.accept(sock);           //wait for message
    std::thread(session, std::move(sock), std::ref(message)/*, std::move(textBox)*/).detach(); //somebody connected; read what they sent to us
    std::cout << "message -> " << message << std::endl;
    usleep(150);  //there is mistake. message here is still empty, because thread doesn't finish by that time. delay added; to be fixed. 
    textBox->append(QString::fromStdString(message));
    log.update();
    log.repaint();
    log.show();
    qApp->processEvents();
//     log.processEvents();
//     iffunction(message);
//     usleep(10);
    std::cout << "2message -> " << message << std::endl;
  }
}
