#include <iostream>
#include <queue>
#include "MScene.hpp"
#include "MStack.hpp"

using namespace std;
using namespace infinity;
using namespace infinity::MScene;


static inline std::int32_t manhattan_distance(const Point& p, const Point& q) {
	return std::abs(q.x - p.x) + std::abs(q.y - p.y);
}

bool astar(const Point& begin_position, const Point& end_position,SceneMap &sceneMap) {

	std::priority_queue<MScene::Point, std::vector<Point>, less<Point>>visit_p_queue{};
	std::vector<Point*>heap_list{};
	visit_p_queue.push(begin_position);
	
	while (!visit_p_queue.empty()) {

		if (visit_p_queue.top() == end_position) {
			setlinestyle(PS_SOLID, 4);
			setlinecolor(YELLOW);
			Point temp_visit_back(visit_p_queue.top());
			while (temp_visit_back.previous) {
				auto offset = sceneMap.boxSize() >> 1;
				Point line_begin = sceneMap.getRenderPosition((*temp_visit_back.previous)) + Point(offset, offset);
				Point line_end = sceneMap.getRenderPosition((temp_visit_back)) + Point(offset, offset);

				line(line_begin.x, line_begin.y, line_end.x, line_end.y);
				temp_visit_back = *(temp_visit_back.previous);
			}
			return true;
		}
		
		auto around_visit_list = sceneMap.getAround(visit_p_queue.top());
		for (auto& iterator_visit : around_visit_list) {

			const Point& current_queue_front = visit_p_queue.top();
			auto ptr_node = new Point(current_queue_front);
			heap_list.push_back(ptr_node);

			iterator_visit.previous = ptr_node;
			iterator_visit.weight = ptr_node->weight + 1;

			iterator_visit.cost = manhattan_distance(iterator_visit, end_position) + iterator_visit.weight;//dis + step = cost;
			visit_p_queue.push(iterator_visit);
			
			BeginBatchDraw();

			sceneMap.setState(iterator_visit, SceneMap::STATE_TYPE::visit);
			sceneMap.render();
			sceneMap.redner(iterator_visit, BLUE);

			sceneMap.redner(begin_position, GREEN);
			sceneMap.redner(end_position, GREEN);

			EndBatchDraw();
		}
		visit_p_queue.pop();
	}
	for (auto& ptr_object : heap_list)delete ptr_object;
	return false;
}

int main(int argc, char* argv[], char* envp[]) {


	static const unsigned int scene_width = 40;
	static const unsigned int scene_height = 20;
	static const unsigned int scene_boxSize = 20;

	initgraph(scene_width * scene_boxSize + 10, scene_height * scene_boxSize + 10);

	SceneMap scene_map(scene_width, scene_height, scene_boxSize);

	const Point self_begin = Point(0, 0);
	const Point self_end = Point(scene_width - 1, scene_height - 1);

	scene_map.setRandBar(vector<Point>({ self_begin, self_end }));

	if (!astar(self_begin, self_end, scene_map))
		MessageBox(GetHWnd(), "not found!", "Astar:fail!", MB_OK);

	getchar();
	closegraph();

	return 0;
}