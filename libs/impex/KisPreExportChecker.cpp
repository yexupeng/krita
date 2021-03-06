/* This file is part of the KDE project
 * Copyright (C) 2016 Boudewijn Rempt <boud@valdyas.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "KisPreExportChecker.h"
#include "KisExportCheckBase.h"
#include "KisExportCheckRegistry.h"

#include <kis_image.h>

KisPreExportChecker::KisPreExportChecker()
{
    KisExportCheckRegistry::instance();
}

bool KisPreExportChecker::check(KisImageSP image, QMap<QString, KisExportCheckBase*> filterChecks)
{
    bool doPerLayerChecks = false;
    if (filterChecks.contains("MultiLayerCheck") && filterChecks["MultiLayerCheck"]->check(image) == KisExportCheckBase::SUPPORTED) {
        doPerLayerChecks = true;
    }

    Q_FOREACH(const QString &id, KisExportCheckRegistry::instance()->keys()) {

        KisExportCheckFactory *factory = KisExportCheckRegistry::instance()->get(id);
        KisExportCheckBase *check = factory->create(KisExportCheckBase::SUPPORTED);

        if (!doPerLayerChecks && check->perLayerCheck()) {
            continue;
        }

        if (check->checkNeeded(image)) {
            if (!filterChecks.contains(id)) {
                m_warnings << check->warning();
            }
            else {
                KisExportCheckBase *filterCheck = filterChecks[id];
                KisExportCheckBase::Level level = filterCheck->check(image);
                QString warning = filterCheck->warning();

                if (level == KisExportCheckBase::PARTIALLY) {
                    m_warnings << warning;
                }
                else if (level == KisExportCheckBase::UNSUPPORTED) {
                    m_errors << warning;
                }
                else {
                    continue;
                }
            }
        }
        delete check;
    }
    return m_warnings.isEmpty() && m_errors.isEmpty();
}

QStringList KisPreExportChecker::errors() const
{
    return m_errors;
}


QStringList KisPreExportChecker::warnings() const
{
    return m_warnings;
}

