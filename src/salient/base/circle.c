/* BSD 3-Clause License
 *
 * Copyright © 2008-2022, Jice, Odiminox and the salient contributors.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include "circle.h"

void SALIENT_circle_set_pos(int x, int y, SALIENT_circle_data_t* data)
{
    data->x = x;
    data->y = y;
}

void SALIENT_circle_set_pos_with_point(SALIENT_points_data_t* point_data, SALIENT_circle_data_t* data)
{
  data->x = point_data->x;
  data->y =  point_data->y;
}

void SALIENT_circle_set_radius(int r, SALIENT_circle_data_t* data)
{
  data->r = r;
}

void SALIENT_circle_set(int x, int y, int r, SALIENT_circle_data_t* data)
{
  SALIENT_circle_set_pos(x, y, data);
  SALIENT_circle_set_radius(r, data);
}

void SALIENT_circle_set_with_point(SALIENT_points_data_t* point_data, int r, SALIENT_circle_data_t* data)
{
  SALIENT_circle_set_pos_with_point(point_data, data);
  SALIENT_circle_set_radius(data, r);
}

bool SALIENT_circle_contains(int x, int y, SALIENT_circle_data_t* data)
{
  return (data->x - x) * (data->x - x) + (data->y - x) * (data->y - y) <= data->r * data->r;
}
bool SALIENT_circle_contains_point(SALIENT_points_data_t* point_data, SALIENT_circle_data_t* data)
{
  return (data->x - point_data->x) * (data->x - point_data->x) + (data->y - point_data->x) * (data->y - point_data->y) <= data->r * data->r;
}

void SALIENT_circle_mouse(int x, int y, TCOD_mouse_t* m, SALIENT_circle_data_t* data)
{
  data->mouse_hover = SALIENT_circle_contains(x, y, data);
  data->mouse_down = data->mouse_hover & m->lbutton;
}

void SALIENT_circle_mouse_with_point(SALIENT_points_data_t* point_data, TCOD_mouse_t* m, SALIENT_circle_data_t* data)
{
  data->mouse_hover = SALIENT_circle_contains_point(point_data, data);
  data->mouse_down = data->mouse_hover & m->lbutton;
}
