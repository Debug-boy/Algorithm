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

		//�������Χ�б������Ѿ��ҵ���ֱ�ӿ����˳���
		auto queryIterator = find(aroundList.cbegin(), aroundList.cend(), end_pos);

		if (queryIterator != aroundList.cend()) {
			//find path!
			mPointStack.push(end_pos);

			//�ɹ��ҵ�����Ⱦ·��
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
			//��ΪaroundListȫ�ǿ��ߵ����ݣ��ͼ���Ŀ����б�ȫ�����Ϊ�߹�
			auto elementList = find(openList.cbegin(), openList.cend(),iterator);
			
			//����ڿ����б������Ҳ�������㣬˵������ʹ��
			if (elementList == openList.cend()) {

				sceneMap.setState(iterator, SceneMap::STATE_TYPE::visit);//��ǵ�״̬Ϊ���ʣ���Ⱦ������Զ���ɫ

				BeginBatchDraw();

				sceneMap.render();//����Ⱦȫ����ͼ
				sceneMap.redner(begin_pos, GREEN);//�����Կ�ʼ��ͽ�������Ⱦ
				sceneMap.redner(end_pos, GREEN);

				EndBatchDraw();

				mPointStack.push(iterator);
				openList.push_back(iterator);//���������ҵ��ĵ�ȫ�����뵽���ߵ��ջ�б�
				firstNormalAppend = true;
				//SleepEx(10, false);
				break;//ֻҪ�ҵ�һ���㣬������������������������֤��·����������
			}
		}

		//���û��һ����ȷ����ӵ�ջ�����ݣ���˵����ǰ��ǰջ�����ȡ����Χ��û���õģ�Ҳ����˵��ǰջ������ȡ
		//�Ƚ��ɿ�ȡ�ĵ���뵽�����б���pop����
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