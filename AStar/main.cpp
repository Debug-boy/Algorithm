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
	std::vector<std::shared_ptr<Point>>heap_list;
	visit_p_queue.push(begin_position);
	
	while (!visit_p_queue.empty()) {

		if (visit_p_queue.top() == end_position) {
			return true;
		}
		
		auto around_visit_list = sceneMap.getAround(visit_p_queue.top());
		for (auto& iterator_visit : around_visit_list) {

			const Point& current_queue_front = visit_p_queue.top();
			std::shared_ptr<Point>ptr_node = std::make_shared<Point>(current_queue_front);//会自动拷贝一个副本点
			heap_list.push_back(ptr_node);

			ptr_node.get()->previous = current_queue_front.previous;
			iterator_visit.previous = ptr_node.get();
			iterator_visit.weight = ptr_node.get()->weight + 1;
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
	for (auto& ptr_object : heap_list) ptr_object.unique();
	return false;
}

int main(int argc, char* argv[], char* envp[]) {


	static const unsigned int scene_width = 40;
	static const unsigned int scene_height = 20;
	static const unsigned int scene_boxSize = 30;

	initgraph(scene_width * scene_boxSize + 10, scene_height * scene_boxSize + 10);

	SceneMap scene_map(scene_width, scene_height, scene_boxSize);

	const Point self_begin = Point(0, 0);
	const Point self_end = Point(scene_width - 1, scene_height - 1);

	scene_map.setRandBar(vector<Point>({ self_begin, self_end }));

	if (!astar(self_begin, self_end, scene_map))
		MessageBox(GetHWnd(), "not found!", "BFS:fail!", MB_OK);

	getchar();
	closegraph();

	return 0;
}