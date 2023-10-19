/* Copyright (C) 2023 Chupligin Sergey <neochapay@gmail.com>
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Nemo Mobile nor the names of its contributors
 *     may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "dbusadaptor.h"
#include "qquickwindow.h"

#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusConnectionInterface>

DBusAdaptor::DBusAdaptor(QQuickWindow* window)
    : QDBusAbstractAdaptor(window)
    , m_window(window)
{
    QDBusConnection sessionbus = QDBusConnection::sessionBus();

    if (sessionbus.interface()->isServiceRegistered("org.nemomobile.qmlmessages")) {
        qWarning() << "Second start of glacier dialler";
        QCoreApplication::quit();
    }

    QDBusConnection::sessionBus().registerService("org.nemomobile.qmlmessages");
    if (!QDBusConnection::sessionBus().registerObject("/", m_window)) {
        qWarning() << Q_FUNC_INFO << "Cannot register DBus object!";
    }
}

void DBusAdaptor::showGroupsWindow()
{
    m_window->raise();
}

void DBusAdaptor::showGroupsWindow(const QStringList& a)
{
    QMetaObject::invokeMethod(m_window, "startSMS", Q_ARG(QStringList, a));
    m_window->raise();
}

void DBusAdaptor::startConversation(const QString& localUid, const QString& remoteUid, bool show)
{
    QMetaObject::invokeMethod(m_window, "showConversation", Q_ARG(QVariant, localUid), Q_ARG(QVariant, remoteUid));
    if (show) {
        m_window->raise();
    }
}

void DBusAdaptor::startSMS(const QString& phoneNumber)
{
    QMetaObject::invokeMethod(m_window, "startSMS", Q_ARG(QVariant, phoneNumber));
    m_window->raise();
}
