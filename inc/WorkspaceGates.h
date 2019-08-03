/* -*-c++-*-

  This file is part of the IC reverse engineering tool degate.

  Copyright 2008, 2009, 2010 by Martin Schobert

  Degate is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version.

  Degate is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with degate. If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef __WORKSPACEGATES_H__
#define __WORKSPACEGATES_H__

#include "WorkspaceElement.h"

namespace degate
{
	class WorkspaceGates : public WorkspaceElement
	{
	public:
		WorkspaceGates(QWidget* new_parent);
		~WorkspaceGates();

		/*
		 * Init all background OpenGL routine (vbo).
		 */
		void init() override;

		/*
	     * Update the background (all textures are reloaded).
	     */
		void update() override;

		/*
	     * Draw the background (all tiles will be draw).
	     */
		void draw(const QMatrix4x4& projection) override;


	private:
		/*
		 * Create on gate.
		 */
		void create_gate(Gate_shptr& gate, unsigned indice);

	};
}

#endif