/*
* Umbra
* Copyright (c) 2009 Dominik Marczuk, Jice
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * The names of Dominik Marczuk or Jice may not be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY DOMINIK MARCZUK & JICE ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL DOMINIK MARCZUK OR JICE BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "main.hpp"
#include "gameview.hpp"
#include <stdio.h>

GameView::GameView() {
	ship = Ship();
	elapsedTime = 0;
	int i, j;
	for (i = 10; i < MAXX-10; i += 5) for (j = 10; j < 25; j += 2)
		entities.push(new Alien(Point(i,j)));
	for (i = 0; i < MAXX; i++) for (j = 0; j < MAXY; j++)
		alienMap[i][j] = NULL;
}

void GameView::keyboard(TCOD_key_t &key) {
	if (key.vk == TCODK_LEFT) ship.move(-1,0,elapsedTime);
	else if (key.vk == TCODK_RIGHT) ship.move(1,0,elapsedTime);
	else if (key.vk == TCODK_SPACE) ship.shoot(&entities,elapsedTime);
}

bool GameView::update() {
	elapsedTime = TCODSystem::getElapsedMilli();
	//move entities. Shots that go off the screen (move returns false) are removed.
	Entity **a, **b;
	for (a = entities.begin(); a != entities.end(); a++)
		if ((*a)->move(0,0,elapsedTime) == false) {
			removeList.push(*a);
			(*a)->removed = true;
		}
	//detect collisions
	int i, j;
	for (a = entities.begin(); a != entities.end(); a++) {
		if ((*a)->type != ENTITY_BULLET)
			continue;
		else {
			for (b = entities.begin(); b != entities.end(); b++) {
				if ((*b)->type != ENTITY_ALIEN)
					continue;
				else if ((*b)->area.contains((*a)->coords)) {
					if ((*a)->removed == false) removeList.push(*a);
					if ((*b)->removed == false) removeList.push(*b);
					(*a)->removed = (*b)->removed = true;
				}
			}
		}
	}
	//eliminate the removed entities
	for (a = removeList.begin(); a != removeList.end(); a++)
		entities.remove(*a);
	return true;
}

void GameView::render() {
	ship.render();
	Entity ** it;
	for (it = entities.begin(); it != entities.end(); it++)
		(*it)->render();
}
