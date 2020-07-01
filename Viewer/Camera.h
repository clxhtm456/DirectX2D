#ifndef __CAMERA_H__
#define __CAMERA_H__

class Camera : public Node
{
public:
	Camera();
	virtual ~Camera();

	static Camera* Create();

	static Camera* MainCamera();

	bool Init();
	void SetView(D3DXMATRIX& V) { _view = V; }
	void SetProjection(D3DXMATRIX& P) { _projection = P; }
	void SetMgnfc(float val);
	float GetMgnfc() const {return mgnfc;}
	D3DXMATRIX* GetView() { return &_view; }

	D3DXMATRIX* GetProjection() { return &_projection; }

	void SetDeleteCallback(const std::function<void(Camera*)>& callback);
	void AddMoveCallback(const function<void(float,float)>& func);

	virtual void Update();
	virtual void Render();

protected:
	static Camera* _mainCamera;
	float depth;
	float mgnfc;

	D3DXMATRIX _view;
	D3DXMATRIX _projection;

	std::function<void(Camera*)> deleteCallback;
private:
	D3DXVECTOR2 oldPosition;

	vector<function<void(float,float)>> moveCallbackList;
};

#endif