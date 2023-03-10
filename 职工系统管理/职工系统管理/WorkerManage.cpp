#include "workerManager.h"

WorkerManager::WorkerManager()
{
	//1.文件不存在
	ifstream ifs;
	ifs.open(FILENAME, ios::in);//读文件
	if (!ifs.is_open())//判断是否打开成功
	{
		cout << "文件不存在" << endl;
		//初始化属性
		this->m_EmpNum = 0;
		this->m_EmpArray = NULL;//初始化数组
		this->m_FileIsEmpty = true;//初始化文件为空标志
		ifs.close();
		return;
	}
	//2.文件存在，但是内容为空
	char ch;
	ifs >> ch;      //先读一个字符
	if (ifs.eof())
	{
		cout << "文件内容为空！" << endl;
		//初始化属性
		this->m_EmpNum = 0;
		this->m_EmpArray = NULL;
		this->m_FileIsEmpty = true;
		ifs.close();
		return;
	}
	//3.文件存在，且内容不为空
	int num = this->get_EmpNum();
	cout << "职工的人数为：" << num << endl;
	this->m_EmpNum = num;
	this->m_EmpArray = new Worker * [this->m_EmpNum];//创建一个堆区的数组，将文件的数据，存到数组中
	this->init_Emp();

	//for (int i = 0;i < this->m_EmpNum;i++)
	//{
	//	cout << "职工编号：" << this->m_EmpArray[i]->m_Id
	//		<< "姓名：" << this->m_EmpArray[i]->m_Name
	//		<< "部门：" << this->m_EmpArray[i]->m_DeptId << endl;
	//}
}
void WorkerManager::Show_Menu()
{
	cout << "*****************************************" << endl;
	cout << "********  欢迎使用职工管理系统！ ********" << endl;
	cout << "************  0.退出管理程序  ***********" << endl;
	cout << "************  1.增加职工信息  ***********" << endl;
	cout << "************  2.显示职工信息  ***********" << endl;
	cout << "************  3.删除离职职工  ***********" << endl;
	cout << "************  4.修改职工信息  ***********" << endl;
	cout << "************  5.查找职工信息  ***********" << endl;
	cout << "************  6.按照编号排序  ***********" << endl;
	cout << "************  7.清空所有文档  ***********" << endl;
	cout << "*****************************************" << endl;
	cout << endl;
}

void WorkerManager::ExitSystem()
{
	cout << "欢迎下次使用" << endl;
	system("pause");
	exit(0);
}

void WorkerManager::Add_Emp()
{
	cout << "请输入添加职工数量：" << endl;
	int addNum = 0;
	cin >> addNum;
	if (addNum > 0)
	{
		int newSize = this->m_EmpNum + addNum;

		//开辟新的空间
		Worker** newSpace = new Worker * [newSize];
		//将原来空间下数据，拷贝到新空间下
		if (this->m_EmpArray != NULL)
		{
			for (int i = 0; i < this->m_EmpNum; i++)
			{
				newSpace[i] = this->m_EmpArray[i];
			}
		}
		//批量添加新数据
		for (int i = 0; i < addNum; i++)
		{
			int id;
			string name;
			int dSelect;
			cout << "请输入第 " << i + 1 << " 个新职工编号：" << endl;
			cin >> id;

			cout << "请输入第 " << i + 1 << " 个新职工姓名：" << endl;
			cin >> name;

			cout << "请选择该职工岗位：" << endl;
			cout << "1、普通职工" << endl;
			cout << "2、经理" << endl;
			cout << "3、老板" << endl;
			cin >> dSelect;

			Worker* worker = NULL;
			switch (dSelect)
			{
			case 1:
				worker = new Employee(id, name, 1);
				break;
			case 2:
				worker = new Manager(id, name, 2);
				break;
			case 3:
				worker = new Boss(id, name, 3);
				break;
			default:
				break;
			}
			//将创建的职工指针，保存到数组中
			newSpace[this->m_EmpNum + i] = worker;
		}
		//释放原有的空间
		delete[] this->m_EmpArray;
		//更改新空间的指向
		this->m_EmpArray = newSpace;
		//更新新的职工的人数
		this->m_EmpNum = newSize;
		//更新职工不为空的标志
		this->m_FileIsEmpty = false;
		cout << "成功添加" << addNum << "名新职工" << endl;
		//保存数据到文件中
		this->save();
	}
	else
	{
		cout << "输入数据有误" << endl;
	}
	system("pause");
	system("cls");
}

//保存文件
void WorkerManager::save()
{
	ofstream ofs;//用输出的方式打开文件-写文件
	ofs.open(FILENAME, ios::out);
	for (int i = 0; i < this->m_EmpNum; i++)
	{
		ofs << this->m_EmpArray[i]->m_Id << " "
			<< this->m_EmpArray[i]->m_Name << " "
			<< this->m_EmpArray[i]->m_DeptId << endl;
	}
	ofs.close();
}

//统计文件中的数据
int WorkerManager::get_EmpNum()
{
	ifstream ifs;
	ifs.open(FILENAME, ios::in);//打开-读

	int id;
	string name;
	int dId;

	int num = 0;//统计人数
	while (ifs >> id && ifs >> name && ifs >> dId)
	{
		num++;
	}
	return num;
}

//
void WorkerManager::init_Emp()
{
	ifstream ifs;
	ifs.open(FILENAME, ios::in);

	int id;
	string name;
	int dId;

	int index = 0;
	while (ifs >> id && ifs >> name && ifs >> dId)
	{
		Worker* worker = NULL;
		if (dId == 1)//普通职工
		{
			worker = new Employee(id, name, dId);
		}
		else if (dId == 2)//经理
		{
			worker = new Manager(id, name, dId);
		}
		else//boss
		{
			worker = new Boss(id, name, dId);
		}
		this->m_EmpArray[index] = worker;
		index++;
	}
	ifs.close();//关闭文件
}

//展示职工的信息
void WorkerManager::Show_Emp()
{
	if (this->m_FileIsEmpty)
	{
		cout << "文件不存在或者记录为空！" << endl;
	}
	else
	{
		for (int i = 0; i < m_EmpNum; i++)
		{
			//利用多态调用程序接口
			this->m_EmpArray[i]->showInfo();
		}
	}
	system("pause");
	system("cls");
}

//删除职工
void WorkerManager::Del_Emp()
{
	if (this->m_FileIsEmpty)
	{
		cout << "文件不存在或记录为空！" << endl;
	}
	else
	{
		cout << "请输入想要删除的职工编号：" << endl;
		int id = 0;
		cin >> id;
		int index = this->isExist(id);//判断该职工是否存在
		if (index != -1)
		{
			for (int i = index; i < this->m_EmpNum - 1; i++)
			{
				this->m_EmpArray[i] = this->m_EmpArray[i + 1];//后面的覆盖前面的，数据前移
			}
			this->m_EmpNum--;//更新数组中记录的人员个数
			this->save();
			cout << "删除成功" << endl;
		}
		else
		{
			cout << "删除失败，未找到该职工" << endl;
		}
	}
	system("pause");
	system("cls");
}


int WorkerManager::isExist(int id)
{
	int index = -1;
	for (int i = 0; i < this->m_EmpNum; i++)
	{
		if (this->m_EmpArray[i]->m_Id == id)
		{
			index = i;
			break;
		}
	}
	return index;
}


//修改职工信息
void WorkerManager::Mod_Emp()
{
	if (this->m_FileIsEmpty)
	{
		cout << "文件不存在或记录为空" << endl;
	}
	else
	{
		cout << "请输入修改的职工编号：" << endl;
		int id;
		cin >> id;
		int ret = this->isExist(id);
		if (ret != -1)//查找成功
		{
			delete this->m_EmpArray[ret];//先释放原来的堆区数据
			//创建新的
			int newId = 0;
			string newName = "";
			int dSelect = 0;

			cout << "查到：" << id << "号职工，请输入新职工号：" << endl;
			cin >> newId;
			cout << "请输入新姓名：" << endl;
			cin >> newName;
			cout << "请输入岗位：" << endl;
			cout << "1、普通职工" << endl;
			cout << "2、经理" << endl;
			cout << "3、老板" << endl;
			cin >> dSelect;
			Worker* worker = NULL;//创建父类指针
			switch (dSelect)//三种情况
			{
			case 1:
				worker = new Employee(newId, newName, dSelect);
				break;
			case 2:
				worker = new Manager(newId, newName, dSelect);
				break;
			case 3:
				worker = new Boss(newId, newName, dSelect);
				break;
			default:
				break;
			}
			//更新到维护的数组中
			this->m_EmpArray[ret] = worker;
			cout << "修改成功" << endl;

			this->save();//保存到文件中
		}
		else
		{
			cout << "修改失败！查无此人" << endl;
		}
	}
	system("pause");
	system("cls");
}

//查找职工：通过两种方式查找，编号和姓名
void WorkerManager::Find_Emp()
{
	if (this->m_FileIsEmpty)
	{
		cout << "文件不存在或者记录为空" << endl;
	}
	else
	{
		cout << "请输入查找的方式" << endl;
		cout << "1.按照职工编号查找" << endl;
		cout << "2.按照职工姓名查找" << endl;
		int select = 0;
		cin >> select;
		//1.按照职工编号查找
		if (select == 1)
		{
			int id;
			cout << "请输入查找的职工编号：" << endl;
			cin >> id;
			int ret = isExist(id);
			if (ret != -1)
			{
				cout << "查找成功，该职工信息如下：" << endl;
				this->m_EmpArray[ret]->showInfo();
			}
			else
			{
				cout << "查找失败，查无此人" << endl;
			}
		}
		//2.按照职工姓名查找
		else if (select == 2)
		{
			string name;
			cout << "请输入查找的姓名：" << endl;
			cin >> name;

			bool flag = false;//标识初始化

			for (int i = 0; i < m_EmpNum; i++)
			{
				if (this->m_EmpArray[i]->m_Name == name)
				{
					cout << "查找成功，职工编号为："<< this->m_EmpArray[i]->m_Id
						<< "号职工信息如下：" << endl;
					flag = true;
					this->m_EmpArray[i]->showInfo();//查找成功并显示
				}
			}
			if (flag = false)
			{
				cout << "查找失败，查无此人" << endl;
			}
		}
		else
		{
			cout << "输入的选项有误" << endl;
		}
	}
	system("pause");
	system("cls");
}

//选择排序：按照职工编号
void WorkerManager::Sort_Emp()
{
	if (this->m_FileIsEmpty)
	{
		cout << "文件不存在或者记录为空" << endl;
		system("pause");
		system("cls");
	}
	else
	{
		cout << "请选择排序方式：" << endl;
		cout << "1.按照职工号进行升序" << endl;
		cout << "2.按照职工号进行降序" << endl;

		int select = 0;
		cin >> select;
		for (int i = 0; i < m_EmpNum; i++)
		{
			int minOrMax = i;//中枢元素
			for (int j = i + 1; j < this->m_EmpNum; j++)
			{
				if (select == 1)//升序
				{
					if (this->m_EmpArray[minOrMax]->m_Id > this->m_EmpArray[j]->m_Id)
					{
						minOrMax = j;
					}
				}
				else//降序
				{
					if (this->m_EmpArray[minOrMax]->m_Id < this->m_EmpArray[j]->m_Id)
					{
						minOrMax = j;
					}
				}
			}
			//判定一开始认定min or max是不是计算的最大最小值，如果不是则交换数据
			if (i != minOrMax)
			{
				Worker* temp = this->m_EmpArray[i];
				this->m_EmpArray[i] = this->m_EmpArray[minOrMax];
				this->m_EmpArray[minOrMax] = temp;
			}
		}
		cout << "排序成功，排序后的结果为：" << endl;
		this->save();
		this->Show_Emp();
	}
}

//清空文件
void WorkerManager::Clean_File()
{
	cout << "确定清空！" << endl;
	cout << "1.确定    2.返回" << endl;

	int select = 0;
	cin >> select;

	if (select == 1)
	{
		//打开模式 ios::trunc如果存在删除文件并重新创建
		ofstream ofs(FILENAME, ios::trunc);
		ofs.close();

		if (this->m_EmpArray != NULL)
		{
			//删除堆区的每个职工对象
			for (int i = 0; i < this->m_EmpNum; i++)
			{
				delete this->m_EmpArray[i];
				this->m_EmpArray[i] = NULL;
			}
			//删除堆区数组指针
			delete[] this->m_EmpArray;
			this->m_EmpArray = NULL;
			this->m_EmpNum = 0;
			this->m_FileIsEmpty = true;
		}
		cout << "清空成功" << endl;
	}
	system("pause");
	system("cls");
}

WorkerManager::~WorkerManager()
{
	if (this->m_EmpArray != NULL)
	{
		for (int i = 0; i < this->m_EmpNum; i++)
		{
			if (this->m_EmpArray[i] != NULL)
			{
				delete this->m_EmpArray[i];
			}
		}
		delete[] this->m_EmpArray;
		this->m_EmpArray = NULL;
	}
}