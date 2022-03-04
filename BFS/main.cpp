#include <iostream>
#include <queue>
#include <algorithm>
#include "MScene.hpp"
#include "MStack.hpp"

using namespace std;
using namespace infinity;
using namespace infinity::MScene;


int distance(const Point& p, const Point& q) {
	int delta_x = q.x - p.x;
	int derta_y = q.y - p.y;
	return abs(delta_x) + abs(derta_y);
}

bool bfs(const Point& begin_position, const Point& end_position, SceneMap& sceneMap) {

	std::queue<Point>visitPointOpenList{};//current visited point!
	std::vector<Point*>makeHeapList{};//record heap point; 
	
	visitPointOpenList.push(begin_position);
	visitPointOpenList.front().previous = nullptr;//The previous node of the first node is nullptr!

	while (!visitPointOpenList.empty()) {

		std::vector<Point>aroundNormalvisit = sceneMap.getAround(visitPointOpenList.front());

		if (visitPointOpenList.front() == end_position) {

			//成功找到后渲染路线
			setlinestyle(PS_SOLID, 4);
			setlinecolor(YELLOW);
			while (visitPointOpenList.front().previous) {
				auto offset = sceneMap.boxSize() >> 1;
				Point line_begin = sceneMap.getRenderPosition((*visitPointOpenList.front().previous)) + Point(offset, offset);
				Point line_end = sceneMap.getRenderPosition(visitPointOpenList.front()) + Point(offset, offset);

				line(line_begin.x, line_begin.y, line_end.x, line_end.y);
				visitPointOpenList.front() = (*visitPointOpenList.front().previous);//回溯上一个节点
			}

			MessageBox(GetHWnd(), "find!", "BFS:success!", MB_OK);
			return true;
		}

		for (auto& iteratorVisited : aroundNormalvisit) {

			const Point &currentFront = visitPointOpenList.front();

			//把 当前的队首节点重新动态生成拷贝一份，因为访问的周围的节点都是队首节点的子节点
			auto lpNewPoint = new Point(currentFront.x, currentFront.y, currentFront.weight,currentFront.previous);
			makeHeapList.push_back(lpNewPoint);

			//lpNewPoint->previous = currentFront.previous;//将新拷贝出来的队首节点 重新指向 父节点(当前队首节点)

			iteratorVisited.previous = lpNewPoint;//将获取到的周围可访问节点的父节点设置为 new出来的节点
			iteratorVisited.weight = currentFront.weight + 1;

			visitPointOpenList.push(iteratorVisited);

			BeginBatchDraw();
			sceneMap.setState(iteratorVisited, SceneMap::STATE_TYPE::visit);
			sceneMap.render();
			sceneMap.redner(iteratorVisited, BLUE);

			sceneMap.redner(begin_position, GREEN);
			sceneMap.redner(end_position, GREEN);
			EndBatchDraw();
			//SleepEx(100, false);
		}
		visitPointOpenList.pop();
	}
	//all make alloc free!
	for (auto lpNewPoint : makeHeapList)delete lpNewPoint;
	return false;
}


int main(int argc, char* argv, char* envp) {

	static const unsigned int scene_width = 40;
	static const unsigned int scene_height = 20;
	static const unsigned int scene_boxSize = 30;

	initgraph(scene_width * scene_boxSize + 10, scene_height * scene_boxSize + 10);

	SceneMap scene_map(scene_width, scene_height, scene_boxSize);

	const Point self_begin = Point(0, 0);
	const Point self_end = Point(scene_width - 1, scene_height - 1);

	scene_map.setRandBar(vector<Point>({ self_begin, self_end }));

	if (!bfs(self_begin, self_end, scene_map))
		MessageBox(GetHWnd(), "not found!", "BFS:fail!", MB_OK);

	getchar();
	closegraph();
	return 0;
}