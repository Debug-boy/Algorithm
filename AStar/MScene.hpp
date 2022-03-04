#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <easyx.h>

namespace infinity {

	template <typename T>
	bool findVector(std::vector<T>dst, T v) {
		for (const auto& value_object : dst) {
			if ((T)value_object == v) {
				return true;
			}
		}
		return false;
	}

	namespace MScene {

		struct Point {
			std::int32_t x, y;
			std::int32_t weight;
			std::int32_t cost;
			MScene::Point *previous;

			explicit Point() :x(), y(), weight(),previous(),cost() {}

			Point(int x, int y, int weight = 0, Point* fatherPoint = nullptr) noexcept(true){
				this->x = x;
				this->y = y;
				this->weight = 0;
				this->previous = fatherPoint;
			}

			void set(int re_x, int re_y, int re_weight, Point* father = nullptr) noexcept(true){
				this->x = re_x;
				this->y = re_y;
				this->weight = re_weight;
				this->previous = father;
			}

			void setCost(std::int32_t add_cost) noexcept(true) {
				this->cost = this->weight + add_cost;	
			}

			std::int32_t distance(const Point &p) const noexcept(true){
				return std::abs(p.x - this->x) + std::abs(p.y - this->y);
			}

			bool operator == (const Point& point) const {
				return (this->x == point.x) && (this->y == point.y);
			}

			bool operator != (const Point& point) const {
				return this->x != point.x || this->y != point.y;
			}

			Point operator + (const Point& point) const {
				return Point(this->x + point.x, this->y + point.y);
			}

			Point operator - (const Point& point) const {
				return Point(this->x - point.x, this->y - point.y);
			}

			Point operator ++ () {
				++this->x;
				++this->y;
				return *this;
			}

			Point operator -- () {
				--this->x;
				--this->y;
				return *this;
			}

			Point operator ++ (int) {
				Point temp = *this;
				++this->x;
				++this->y;
				return temp;
			}

			Point operator -- (int) {
				Point temp = *this;
				--this->x;
				--this->y;
				return temp;
			}

			bool operator < (const Point& compare_obj) const noexcept(true){
				return this->cost > compare_obj.cost;
			}

			bool operator > (const Point& compare_obj) const noexcept(true) {
				return this->cost < compare_obj.cost;
			}

		};

		class CoordinatePoint {
		private:
			Point m_position;
			int m_state;

		public:

			CoordinatePoint() :m_position(), m_state() {}

			CoordinatePoint(const Point& position, const int& state) {
				m_position = position;
				m_state = state;
			}

			~CoordinatePoint() = default;

		public:

			Point getPosition() const {
				return m_position;
			}

			int getState() const {
				return m_state;
			}

			void setPosition(const Point& position) {
				this->m_position = position;
			}

			void setState(const int& state) {
				this->m_state = state;
			}

			void reset(const Point& position, const int& state) {
				this->m_position = position;
				this->m_state = state;
			}

		};

		class SceneMap {
		private:
			std::vector <std::vector<CoordinatePoint>>m_data;
			unsigned int m_width;
			unsigned int m_height;
			unsigned int m_boxSize;

		public:
			enum STATE_TYPE { nromal, bar, retain, visit };

			
			explicit SceneMap() : m_data(), m_width(), m_height(),m_boxSize() {}
			
			explicit SceneMap(unsigned int scene_width, unsigned int scene_height,unsigned int boxSize) {
				m_width = scene_width;
				m_height = scene_height;
				m_boxSize = boxSize;

				for (int i = 0; i < scene_width; i++) {
					
					std::vector<CoordinatePoint>temp_row;
					for (int j = 0; j < scene_height; j++)
						temp_row.push_back(CoordinatePoint(Point(i, j), STATE_TYPE::nromal));

					m_data.push_back(temp_row);
					temp_row.clear();
				}

				//w*h
			}

			~SceneMap() {
				m_data.clear();
			}

		public:

			bool checkPointExist(const Point& checkPoint) const{
				//normal
				if (checkPoint.x >= 0 && checkPoint.y >= 0) {
					if (checkPoint.x < m_width && checkPoint.y < m_height) {
						return true;
					}
				}
				return false;//exception
			}

			bool getCoordInfo(const Point &scenePoint,CoordinatePoint& queryInfo) const {
				//normal
				if (checkPointExist(scenePoint))
					queryInfo = m_data[scenePoint.x][scenePoint.y];
				return false;//exception
			}

			void setState(const Point& scenePoint, const unsigned int& state) {
				if (checkPointExist(scenePoint))
					m_data[scenePoint.x][scenePoint.y].setState(state);
			}

			void recoverState(const Point& scenePoint) {
				m_data[scenePoint.x][scenePoint.y].setState(STATE_TYPE::nromal);
			}

			//±£Áôµã
			void setRandBar(const std::vector<Point>&retain) {
				srand(time(NULL));
				for (auto i = 0; i < m_width; i++) {
					for (auto j = 0; j < m_height; j++) {
						Point target_position = m_data[i][j].getPosition();

						if (!findVector<Point>(retain, target_position)) {
							int r = rand() % (m_width / 2) + 1;
							if (r % 3 == 0)
								setState(target_position,STATE_TYPE::bar);
						}
					}
				}

			}

			unsigned int boxSize() const{
				return m_boxSize;
			}

			Point getRenderPosition(const Point& scenePoint) const{
				return Point(scenePoint.x * m_boxSize, scenePoint.y * m_boxSize);
			}

			//up right down left ¡ü ¡ú ¡ý ¡û
			std::vector<Point> getAround(const Point &scenePoint) const{
				static const Point offsets[] = { {-1,0},{0,1},{1,0},{0,-1} };
				std::vector<Point>aroundData;

				for (const Point&offset : offsets) {
					Point point = (Point)scenePoint + offset;
					if (checkPointExist(point)) {
						if (
							m_data[point.x][point.y].getState() == STATE_TYPE::nromal) {
							aroundData.push_back(point);
						}
					}
				}
				return aroundData;
			}

			void redner(const Point& scenePoint, COLORREF color) {
				setfillcolor(color);
				int temp_x = scenePoint.x * m_boxSize;
				int temp_y = scenePoint.y * m_boxSize;
				int temp_w = temp_x + m_boxSize;
				int trmp_h = temp_y + m_boxSize;
				fillrectangle(temp_x, temp_y, temp_w, trmp_h);
			}

			void render() {

				solidrectangle(0, 0, m_width * m_boxSize, m_height * m_boxSize);

				for (int x = 0; x < m_width; x++) {
					for (int y = 0; y < m_height; y++) {
						int temp_x = x * m_boxSize;
						int temp_y = y * m_boxSize;
						int temp_w = temp_x + m_boxSize;
						int trmp_h = temp_y + m_boxSize;

						switch (m_data[x][y].getState())
						{
						case STATE_TYPE::bar:
							setfillcolor(RED);
							break;

							//default
						case STATE_TYPE::nromal:
							setfillcolor(BLACK);
							break;

						case STATE_TYPE::retain:
							setfillcolor(GREEN);
							break;

						case STATE_TYPE::visit:
							setfillcolor(BLUE);
							break;

						default:
							break;
						}
						fillrectangle(temp_x, temp_y, temp_w, trmp_h);
					}
				}
			}
		};
	}
}