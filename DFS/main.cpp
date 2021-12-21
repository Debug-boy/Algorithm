#define _CRT_SECURE_NO_WARNINGS
#include <easyx.h>
#include <vector>
#include <windows.h>
#include <time.h>
#include "MStack.hpp"
#include "MScene.hpp"

using namespace std;
using namespace infinity;
using namespace infinity::MScene;

bool dfs(const Point& begin_pos, const Point& end_pos, SceneMap& sceneMap) {

	std::vector<Point>openList{};
	std::vector<Point>backList{};
	infinity::MStackPlus<Point>mPointStack{};

	mPointStack.push(begin_pos);
	openList.push_back(begin_pos);

	while (!mPointStack.empty()) {
	
		Point currentPoint = mPointStack.stack_top();

		auto aroundList = sceneMap.getAround(currentPoint);

		//如果在周围列表里面已经找到就直接可以退出了
		auto queryIterator = find(aroundList.cbegin(), aroundList.cend(), end_pos);

		if (queryIterator != aroundList.cend()) {
			//find path!
			mPointStack.push(end_pos);

			//成功找到后渲染路线
			setlinestyle(PS_SOLID, 4);
			setlinecolor(YELLOW);
			while (!mPointStack.empty()) {
				auto offset = sceneMap.boxSize() / 2;
				Point line_begin = sceneMap.getRenderPosition((mPointStack.stack_top())) + Point(offset, offset);
				if (mPointStack.pop() && !mPointStack.empty()) {
					Point line_end = sceneMap.getRenderPosition((mPointStack.stack_top())) + Point(offset, offset);
					line(line_begin.x, line_begin.y, line_end.x, line_end.y);
				}
			}

			//for (const auto& point : backList) {
			//	sceneMap.redner(point, CYAN);
			//}

			MessageBox(GetHWnd(), "find","DFS:success", MB_OK);
			return true;
		}

		bool firstNormalAppend = false;
		for (Point& iterator : aroundList) 
		{
			//因为aroundList全是可走的数据，就加入的开放列表，全部标记为走过
			auto elementList = find(openList.cbegin(), openList.cend(),iterator);
			
			//如果在开放列表里面找不到这个点，说明可以使用
			if (elementList == openList.cend()) {

				sceneMap.setState(iterator, SceneMap::STATE_TYPE::visit);//标记点状态为访问，渲染则可以自动换色

				BeginBatchDraw();

				sceneMap.render();//先渲染全部地图
				sceneMap.redner(begin_pos, GREEN);//单独对开始点和结束点渲染
				sceneMap.redner(end_pos, GREEN);

				EndBatchDraw();

				mPointStack.push(iterator);
				openList.push_back(iterator);//将现在所找到的点全部加入到欲走点的栈列表
				firstNormalAppend = true;
				//SleepEx(10, false);
				break;//只要找到一个点，就跳出对这个点进行搜索，保证线路的最快可行性
			}
		}

		//如果没有一次正确的添加到栈顶数据，那说明当前当前栈顶点获取的周围是没有用的，也就是说当前栈顶不可取
		//先将可可取的点加入到回溯列表，在pop掉；
		if (!firstNormalAppend) {
			backList.push_back(currentPoint);
			mPointStack.pop();
			continue;
		}

	}

	//if stack is empty,this sceneMap is not found path!
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

	if(!dfs(self_begin, self_end, scene_map))
		MessageBox(GetHWnd(), "not found!", "DFS:fail!", MB_OK);

	getchar();
	closegraph();
	return 0;
}