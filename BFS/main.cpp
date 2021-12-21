#include <iostream>
#include <queue>
#include <algorithm>
#include "MScene.hpp"

using namespace std;
using namespace infinity;
using namespace infinity::MScene;


bool bfs(const Point& begin_position, const Point& end_position, SceneMap& sceneMap) {

	queue<Point>visitPointList{};
	
	visitPointList.push(begin_position);

	while (!visitPointList.empty()) {

		if (visitPointList.front() == end_position) {
			MessageBox(GetHWnd(), "find target!", "success", MB_OK);
			return true;
		}

		std::vector<Point>aroundNormalvisit = sceneMap.getAround(visitPointList.front());

		for (auto& iteratorVisited : aroundNormalvisit) {
			iteratorVisited.weight = visitPointList.front().weight + 1;
			visitPointList.push(iteratorVisited);
			
			BeginBatchDraw();
			sceneMap.setState(iteratorVisited, SceneMap::STATE_TYPE::visit);
			sceneMap.render();//先渲染整张地图
			sceneMap.redner(iteratorVisited, BLUE);//渲染迭代访问的四周点

			sceneMap.redner(begin_position, GREEN);//单独对开始点和结束点渲染
			sceneMap.redner(end_position, GREEN);
			EndBatchDraw();
		}

		visitPointList.pop();
	}
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
		MessageBox(GetHWnd(), "not found!", "fail!", MB_OK);

	getchar();
	closegraph();
	return 0;
}