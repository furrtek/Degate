/**
 * This file is part of the IC reverse engineering tool Degate.
 *
 * Copyright 2008, 2009, 2010 by Martin Schobert
 * Copyright 2019-2020 Dorian Bachelot
 *
 * Degate is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * Degate is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with degate. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "Core/RuleCheck/RCVBlacklistImporter.h"
#include "Core/Image/ImageHelper.h"

#include <cerrno>
#include <iostream>
#include <stdexcept>
#include <list>

using namespace std;
using namespace degate;

void RCVBlacklistImporter::import_into(std::string const& filename,
                                       RCBase::container_type& blacklist)
{
    if (RET_IS_NOT_OK(check_file(filename)))
    {
        debug(TM, "Problem: file %s not found.", filename.c_str());
        throw InvalidPathException("Can't load gate library from file.");
    }

    std::string directory = get_basedir(filename);

    try
    {
        QDomDocument parser;

        QFile file(QString::fromStdString(filename));
        if (!file.open(QIODevice::ReadOnly))
        {
            debug(TM, "Problem: can't open the file %s.", filename.c_str());
            throw InvalidFileFormatException("The ProjectImporter cannot load the project file. Can't open the file.");
        }

        if (!parser.setContent(&file))
        {
            debug(TM, "Problem: can't parse the file %s.", filename.c_str());
            throw InvalidXMLException("The ProjectImporter cannot load the project file. Can't parse the file.");
        }
        file.close();

        const QDomElement root_elem = parser.documentElement(); // deleted by DomParser
        assert(!root_elem.isNull());

        parse_list(root_elem, blacklist);
    }
    catch (const std::exception& ex)
    {
        std::cout << "Exception caught: " << ex.what() << std::endl;
        throw;
    }
}


void RCVBlacklistImporter::parse_list(QDomElement const elem,
                                      RCBase::container_type& blacklist)
{
    const QDomNodeList rcv_list = elem.elementsByTagName("rc-violation");
    QDomElement e;
    for (int i = 0; i < rcv_list.count(); i++)
    {
        e = rcv_list.at(i).toElement();

        if (!e.isNull())
        {
            object_id_t object_id = parse_number<object_id_t>(e, "object-id");

            const std::string rcv_class(e.attribute("rc-violation-class").toStdString());
            const std::string severity(e.attribute("severity").toStdString());

            RCViolation_shptr rcv(new RCViolation(lmodel->get_object(object_id),
                                                  rcv_class,
                                                  RCViolation::get_severity_from_string(severity)));

            blacklist.push_back(rcv);
        }
    }
}
