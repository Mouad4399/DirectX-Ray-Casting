/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"

#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <unordered_map>
#include <utility>
#include <algorithm>
constexpr double PI = 3.141592653589793238463;

int mapdim = 0;
void raycast(int index,std::pair<int, int> player , std::vector<std::vector<int>> map ,double rotation ,std::vector<double> & raylengths, Graphics& gfx2) {

	// raycasting 

	//how many norm enough to get to Ver
	double stepHor = (std::sin(rotation) > 0) ? 1 : 0;
	double stepVer = (std::cos(rotation) > 0) ? 1 : 0;

	std::pair<double, double> pos = player;

	bool intersCas = false;

	double normToVer = std::abs((pos.first - (std::floor(pos.first / mapdim) + stepVer) * mapdim) / (std::cos(rotation) == 0 ? 6.12323e-17 : std::cos(rotation)));
	double normToHor = std::abs((pos.second - (std::floor(pos.second / mapdim) + stepHor) * mapdim) / (std::sin(rotation) == 0 ? 6.12323e-17 : std::sin(rotation)));

	while ((map[std::floor((pos.second) / mapdim)][std::floor((pos.first) / mapdim)] == 0)) {

		normToVer = std::abs((pos.first - (std::floor(pos.first / mapdim) + stepVer) * mapdim) / (std::cos(rotation) == 0 ? 6.12323e-17 : std::cos(rotation)));
		normToHor = std::abs((pos.second - (std::floor(pos.second / mapdim) + stepHor) * mapdim) / (std::sin(rotation) == 0 ? 6.12323e-17 : std::sin(rotation)));

		if ((pos.first - (std::floor(pos.first / mapdim) + stepVer) * mapdim == 0)) {
			//double helper = stepVer==0 ? -1:
			//std::cout << "heyy Vertical" << std::endl;
			normToVer = std::abs((pos.first - (std::floor(pos.first / mapdim) + stepVer - 1) * mapdim) / (std::cos(rotation) == 0 ? 6.12323e-17 : std::cos(rotation)));

			intersCas = true;
		}

		if (pos.second - (std::floor(pos.second / mapdim) + stepHor) * mapdim == 0) {


			//std::cout << "heyy Horizontal" << std::endl;
			normToHor = std::abs((pos.second - (std::floor(pos.second / mapdim) + stepHor - 1) * mapdim) / (std::sin(rotation) == 0 ? 6.12323e-17 : std::sin(rotation)));
			intersCas = true;
		}

		if (normToVer < normToHor) {
			pos.first += normToVer * std::cos(rotation);
			pos.second += normToVer * std::sin(rotation);
		}
		else {
			pos.first += normToHor * std::cos(rotation);
			pos.second += normToHor * std::sin(rotation);
		}



		/*if ((normToVer == 0 || normToHor == 0)) {
			std::cout << "I am breaking" << std::endl;
			break;
		}*/
		//std::cout << " second  = " << std::floor((pos.second + 10 * std::sin(rotation)) / mapdim) << " , first = " << std::floor((pos.first + 10 * std::cos(rotation)) / mapdim) << std::endl;
		if ((map[std::floor((pos.second + 2 * std::sin(rotation)) / mapdim)][std::floor((pos.first + 2 * std::cos(rotation)) / mapdim)] == 1)) {

			break;
		}
	}


	//std::cout << "------------------finished--------------------------" << std::endl;
	//drawing the ray
	for (double i = 0; i < 2 * PI; i = i + 0.0174533) {

		for (int r = 0; r <= 2; ++r) {
			gfx2.PutPixel(pos.first + std::cos(i)*r, pos.second + std::sin(i)*r, { 255,0,0 });
		}


	}
	for (double i = 0; i < (std::sqrt(std::pow(player.first - pos.first, 2) + std::pow(player.second - pos.second, 2))); ++i) {
		gfx2.PutPixel(player.first + std::cos(rotation)*i, player.second + std::sin(rotation)*i, { 255,0,0 });
	}

	//saving the raylengths
	//needs to be dot producted later with rotation vector
	raylengths[index] = (std::sqrt(std::pow(player.first - pos.first, 2) + std::pow(player.second - pos.second, 2)));


}
Game::Game( MainWindow& wnd , MainWindow& wnd2)
	:
	wnd( wnd ),
	wnd2( wnd2 ),
	gfx( wnd ),
	gfx2( wnd2 )
{
	//gfx2.SW = 40;
	//gfx2.SH = 30;
	//q.push(start);
	//cameFrom[start.first * (Graphics::ScreenWidth/dim) + start.second] = std::make_pair( -1, -1 );

	rotation = 3 * PI / 4; 

	mapdim = dim;
}

void Game::Go()
{
	gfx.BeginFrame();	
	gfx2.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx2.EndFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{


	//if (!q.empty() && pathfounded == false) {
	//	auto x = q.front().first;
	//	auto y = q.front().second;
	//	q.pop();
	//	if (x == target.first && y == target.second) {
	//		pathfounded = true;
	//		//break;
	//	};

	//	for (auto& dir : directions) {
	//		int nx = x + dir.first, ny = y + dir.second;
	//		if (nx >= 0 && ny >= 0 && nx < Graphics::ScreenWidth / dim && ny < Graphics::ScreenHeight / dim && cameFrom.find(nx * Graphics::ScreenWidth / dim + ny) == cameFrom.end()) {
	//			q.push({ nx,ny });
	//			cameFrom[nx * Graphics::ScreenWidth / dim + ny] = { x, y };

	//			//gfx.PutPixel(nx, ny, { 0,255,0 });

	//			for (int j = 0; j < dim; ++j) {

	//				for (int i = 0; i < dim; ++i) {
	//					gfx.PutPixel(nx*dim + j, ny*dim + i, { 0,255,0 });
	//				}
	//			}

	//		}
	//	}
	//}

	auto T = [](double y, double x) {
		double at2 = atan2(y, x);
		//		std::cout<<"test angle : "<<(at2 > 0 ? at2 : (2*PI + at2))<<"\n";
		return (at2 > 0 ? at2 : (2 * PI + at2));
	};

	for (int i = 0; i < 800; ++i) {

		raycast(i,player,map , (PI / (2*800)) * i + (rotation - PI/4), raylengths,gfx2);
	}

	if (wnd.kbd.KeyIsPressed(VK_DOWN)) {

		player.first -= 5*std::cos(rotation);
		player.second -=5*std::sin(rotation);

		for (double i = 0; i < 2 * PI; i = i + 0.0174533) {

			if (map[std::floor((player.second + std::sin(i)*5)/mapdim)][std::floor((player.first + std::cos(i) * 5) / mapdim)] == 1) {
				player.first += 5*std::cos(rotation);
				player.second += 5*std::sin(rotation);

			}

		}

	}

	if (wnd.kbd.KeyIsPressed(VK_UP)) {

		player.first += 5*std::cos(rotation);
		player.second += 5*std::sin(rotation);


		for (double i = 0; i < 2 * PI; i = i + 0.0174533) {

			if (map[std::floor((player.second + std::sin(i) * 5) / mapdim)][std::floor((player.first + std::cos(i) * 5) / mapdim)] == 1) {
				player.first -= 5*std::cos(rotation);
				player.second -= 5*std::sin(rotation);

			}

		}

	}

	if (wnd.kbd.KeyIsPressed(VK_LEFT)) {

		rotation -= 0.0174533*3;
	}

	if (wnd.kbd.KeyIsPressed(VK_RIGHT)) {
		rotation += 0.0174533*3;

	}



}


void Game::ComposeFrame()
{
	// 3d map
	double fc = 0.05;
	double midlv = std::min(50000 / raylengths[(int)(raylengths.size() / 2)] , (double)Graphics::ScreenHeight);
	for (int i = 0; i <raylengths.size(); ++i) {
		/*if (i == 10) {
			break;
		}*/
		for (int n = 0; n < Graphics::ScreenWidth / raylengths.size(); ++n) {

			//double lv = fc / (raylengths[i] * std::cos(0.5*std::abs(rotation + (PI / (2 * raylengths.size())) * i + (rotation - PI / 4))) );

			double lv = std::min(50000 / (raylengths[i] * std::cos((PI / (2 * raylengths.size())) * std::abs((int)(raylengths.size() / 2) - i))), (double)Graphics::ScreenHeight);
			int color = std::max((int)(255*(1- 100/lv)), 20);

			//std::cout << "md : " << midlv << std::endl;
			for (int m = 0; m < lv ; ++m) {
				gfx.PutPixel(( i)*(Graphics::ScreenWidth / raylengths.size())+n,(Graphics::ScreenHeight-lv)/2 + m, color,color,color);
			}

			//color of either the sky or the soil
			for (int j = 0; j < (Graphics::ScreenHeight - lv) / 2; ++j) {
				gfx.PutPixel((i)*(Graphics::ScreenWidth / raylengths.size()) + n, (Graphics::ScreenHeight - lv) / 2 + lv + j, {255,0,0});
			}
		}
	}

	


		//for (int r = 0; r < map[0].size(); ++r) {
	//draw the map

	for (int c = 0; c < map.size(); ++c) {

		for (int r = 0; r < map[0].size(); ++r) {

			if (map[c][r] == 0) {
				continue;
			}

			for (int i = 0; i < mapdim; ++i) {

				for (int j = 0; j < mapdim; ++j) {
					
					gfx2.PutPixel(r*mapdim+j,c*mapdim+i, { 0,255,0 });
				}
			}

		}

	}


	//for (int c = 0; c < map[0].size(); ++c) {

	//	for (int i = 0; i < map.size() * mapdim; ++i) {
	//		gfx2.PutPixel( c * mapdim,i, { 255,255,255 });
	//	}


	//}
	//for (int c = 0; c < map.size(); ++c) {

	//	for (int i = 0; i < map[0].size() * mapdim; ++i) {
	//		gfx2.PutPixel( i,c * mapdim, {255,255,255 });
	//	}


	//}

	// draw the player


	gfx2.PutPixel(player.first, player.second, { 0,0,255 });

	//for (int r = 0; r <= mapdim; ++r) {
	//	gfx.PutPixel(player.first + std::cos(rotation - PI / 4)*r, player.second + std::sin(rotation - PI / 4)*r, { 255,0,0 });
	//}
	//for (int r = 0; r <= mapdim; ++r) {
	//	gfx.PutPixel(player.first + std::cos(rotation + PI / 4)*r, player.second + std::sin(rotation + PI / 4)*r, { 255,0,0 });
	//}
	for (double i = 0; i < 2 * PI; i = i + 0.0174533) {

		for (int r = 0; r <= 5; ++r) {
			gfx2.PutPixel(player.first + std::cos(i)*r, player.second + std::sin(i)*r, { 0,0,255 });
		}



	}

}
