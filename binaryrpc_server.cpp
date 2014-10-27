#include "binaryrpc/binaryrpc_server_p.h"
#include "binaryrpc/binaryrpc_session_p.h"

namespace binaryrpc {

ServerPrivate::ServerPrivate(Server *parent) : parent(parent) {

}

ServerPrivate::~ServerPrivate() {

}

void ServerPrivate::incomingConnection(int socket_descriptor) {
  emit parent->newSession(new Session(socket_descriptor));
}

Server::Server() : p(new ServerPrivate(this)) {

}

Server::~Server() {
  if(p) {
    delete p;
    p = 0;
  }
}

QString Server::getAddress() {
  return p->serverAddress().toString();
}

quint16 Server::getPort() const {
  return p->serverPort();
}

bool Server::listen(const QHostAddress &host, quint16 port) {
  return p->listen(host, port);
}

bool Server::isListening() const {
  return p->isListening();
}

void Server::close() {
  return p->close();
}

QString Server::errorString() const {
  return p->errorString();
}

QAbstractSocket::SocketError Server::serverError() const {
  return p->serverError();
}

} //namespace binaryrpc
