#include "tsfileinfo.h"

Context::Context(const QString sName)
{
    m_sName = sName;
}

Context::Context(const Context &other)
{
    this->m_sName = other.m_sName;
    this->m_lMessage = other.m_lMessage;
}

Context & Context::operator =(const Context &other)
{
    if(this != &other){
        this->m_sName = other.m_sName;
        this->m_lMessage = other.m_lMessage;
    }
    return *this;
}

Context::~Context()
{
    foreach(Message *message,m_lMessage){
        delete message;
    }
}

void Context::setName(const QString &name)
{
    m_sName = name;
}

QString Context::name() const
{
    return m_sName;
}

void Context::addMessage(Message *message)
{
    if(message == Q_NULLPTR){
        return ;
    }
    m_lMessage.append(message);
}

MessageList Context::messageList() const
{
    return m_lMessage;
}

