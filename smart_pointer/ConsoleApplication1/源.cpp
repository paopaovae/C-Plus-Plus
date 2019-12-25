#include<iostream>
#include<string>
#include<memory>
#include<vector>
using namespace std;

int clone1(unique_ptr<int> p);
unique_ptr<int> clone2(int p);

int main(void) {

	
	/*shared_ptr 的初始化方式
	以及get 和 reset new分配的动态内存。
	*/
	
	shared_ptr<int> sPtr1;										//空指针
	shared_ptr<int> sPtr2 = make_shared<int>(10);				// make_shared初始化

	int *p1 = new int(10);
	shared_ptr<int> sPtr3(new int(11));							//普通指针初始化
	shared_ptr<int> sPtr4(p1);									//直接初始化
																//shared_ptr<int> sPtr5 = new int(10);						// 无法将普通指针拷贝初始化

	shared_ptr<int> sPtr6(sPtr3.get());							//虽然sPtr6和sPtr3指向同一块动态内存，在物理意义上是共享对象，但是编译器认为两个是指向不同的对象。
																//因为sPtr3.get返回一个(int *)

	cout << "sPtr3.use_count = " << sPtr3.use_count() << endl;  //==1
	cout << "sPtr6.use_count = " << sPtr6.use_count() << endl;  //==1

	cout << "sPtr3.use_count = " << sPtr3.use_count() << endl;  //==1
	cout << "sPtr4.use_count = " << sPtr4.use_count() << endl;  //==1

	shared_ptr<int> sPtr7 = sPtr4;                              //sPtr7 和 sPtr4共享同一块动态内存
	cout << "sPtr7.use_count = " << sPtr7.use_count() << endl;  //==2  引用次数为2
	sPtr7.reset();

	cout << "sPtr7.use_count = " << sPtr7.use_count() << endl;  //==0 sPtr7.reset之后变为 nullptr，
	cout << "sPtr4.use_count = " << sPtr4.use_count() << endl;  //==1 sPtr4引用次数减1 == 1
	if (sPtr7 == nullptr)
		cout << "sPtr7 == nullptr" << endl;
	else
		cout << "sPtr7 != nullptr" << endl;

	int *p2 = nullptr;
	shared_ptr<int> sPtr8(new int(10));
	sPtr8.reset(new int(10));									// 可以reset一个new 分配的指针
	sPtr8.reset(sPtr2.get());									//可以rest 一个 int *
	sPtr8.reset(p2);
	
	
	shared_ptr<int> sPtr9(new int(5));
	/*
	unique_ptr的使用
	某个时刻只能有一个unique_ptr指向一个给定对象。当unique_ptr销毁时，它所指的对象也被销毁。
	*/
	/*
	//unique_ptr<int> uPtr1;										//空指针
	unique_ptr<int> uPtr2(new int(10));
	//unique_ptr<int> uPtr3 = make_shared<int>(10);				//错误，并能用make_shared方式初始化一个unique_ptr指针
	//unique_ptr<int> uPtr4(uPtr2);								//不能拷贝
	//unique_ptr<int> uPtr5 = uPtr2;							//不能赋值 也就是说不能用一个sPtr去初始化智能指针。
	//shared_ptr<int> sPtr9(uPtr2);								
	unique_ptr<int> uPtr6(uPtr2.get());							//同上述sPtr.get()
	shared_ptr<int> sPtr9(new int(5));
	unique_ptr<int> uPtr7(sPtr9.get());
	if (uPtr2 == nullptr)
		cout << "uPtr2 == nullptr" << endl;
	else {
		cout << "uPtr2 != nullptr" << endl;
		cout << "*uPtr2 = " << *uPtr2 << endl;
		cout << "*uPtr6 = " << *uPtr6 << endl;
	}

	if (sPtr9 == nullptr)
		cout << "sPtr9 == nullptr" << endl;
	else {
		cout << "sPtr9 != nullptr" << endl;
		cout << "sPtr9.use_count = " << sPtr9.use_count() << endl;
		cout << "*sPtr9 = " << *sPtr9 << endl;
		cout << "*uPtr7 = " << *uPtr7 << endl;
	}

	uPtr7.release();											 // 返回指针，并置指针为空
	if (uPtr7 == nullptr)
		cout << "uPtr7 == nullptr" << endl;
	else {
		cout << "uPtr7 != nullptr" << endl;
	}


	if (uPtr6 == nullptr)
		cout << "uPtr6 == nullptr" << endl;
	else {
		cout << "uPtr6 != nullptr" << endl;
	}
	uPtr7.reset(sPtr9.get());								   //uPtr 和 sPtr 可以相互reset对方的get（）返回的指针，但是最好不要这么做。
	uPtr7.reset(uPtr6.release());							   //置uPtr7为uPtr6指向的动态对象，uPtr6为空。
	if (uPtr7 == nullptr)
		cout << "uPtr7 == nullptr" << endl;
	else {
		cout << "uPtr7 != nullptr" << endl;
	}
	if (uPtr6 == nullptr)
		cout << "uPtr6 == nullptr" << endl;
	else {
		cout << "uPtr6 != nullptr" << endl;
	}
	/*
	/*
		上述提到uPtr不能拷贝和赋值，但是作为函数参数或返回值时可以。
	*/
	//int num = clone1(uPtr7);								  // 函数参数不行
	unique_ptr<int> uPtr8 = clone2(6);                        //可以作为返回值返回。

	
	/*
		weak_ptr的使用
		wPtr是一种不控制所指向对象生存周期的智能指针，指向一个sPtr的管理对象。不会改变其引用数目。
		一旦最后一个sPtr被销毁，即使wPtr指向对象，对象还是会被销毁。
	*/

	weak_ptr<int> wPtr1;										//空指针
	weak_ptr<int> wPtr2(sPtr9);									//sPtr初始化
	//weak_ptr<int> wPtr3(sPtr9.get());							// 不能用get返回初始化。
	//weak_ptr<int> wPtr4(uPtr2.get());
	weak_ptr<int> wPtr5 = sPtr9;								// 赋值初始化
	//weak_ptr<int> wPtr6(new int(10));							// 不能用int *初始化。

	
	//if (wPtr5 == nullptr)                                       // 不能作为一个比较对象
	cout << "wPtr5.use_count == " << wPtr5.use_count() << endl;
	if (wPtr5.expired())
		cout << "wPtr5.use_count == " << 0 << endl;
	else
		cout << "wPtr5.use_count == " << wPtr5.use_count() << endl;
	wPtr5.reset();
	cout << "wPtr5.use_count == " << wPtr5.use_count() << endl;


	getchar();
	return 0;

}

//传递一个uPtr参数
int clone1(unique_ptr<int> p) {
	if (p != nullptr)
		return *p;
	else
		return 0;
}

// 局部对象的拷贝
unique_ptr<int> clone2(int p) {
	unique_ptr<int> ret(new int(p));
	return ret;
}