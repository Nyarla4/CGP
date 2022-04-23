////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_


////////////////////////////////////////////////////////////////////////////////
// Class name: InputClass
////////////////////////////////////////////////////////////////////////////////
class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	void Initialize();//초기화만 있다(셧다운은 따로 없음)

	void KeyDown(unsigned int);//키가 눌렸는가
	void KeyUp(unsigned int);//키가 떼졌는가

	bool IsKeyDown(unsigned int);//현재 눌려지고 있는가

private:
	bool m_keys[256];//키들을 담아둘수 있는 배열
};

#endif