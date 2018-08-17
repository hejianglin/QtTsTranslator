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

//////////////////////////////////////////////////////////
/// ContextMap
ContextMap::ContextMap()
    : m_iIndex( - 1)
{

}

ContextMap::~ContextMap()
{
    clear();
}

bool ContextMap::isEmpty() const
{
    return m_iIndex == -1;
}

bool ContextMap::isExists(const QString &name) const
{
    return m_hashName.value(name,-1) != -1;
}

Context *ContextMap::operator [](const QString &name)
{
    return m_listContext.value(m_hashName.value(name));
}

void ContextMap::clear()
{
    foreach(Context *item,m_listContext){
        delete item;
    }

    m_listContext.clear();
    m_hashName.clear();
    m_iIndex = -1;
}
void ContextMap::append(const QString &name,Context *item)
{
    if(m_hashName.value(name,-1) == -1){
        m_hashName.insert(name,++m_iIndex);
        m_listContext.insert(m_iIndex,item);
    }
}

ContextList ContextMap::context() const
{
    return m_listContext;
}

