# PhysX-Vehicle-Demo
> Opengl+PhysX+GLFW+DearImGui

![图片](https://github.com/capp-adocia/Physx-Vehicle-Demo/blob/master/img.png)

## Detail
- 使用Physx实现了车辆运动前进、后退、转弯，但存在一个未知的bug，当我按住前进键后，等待达到最大速度后，此时再任意按下其他键，速度就会突破最大值，继续增加一倍的速度，这是一个十分奇怪的bug，目前为止我仍然未能解决它。

## Attention
> 如果你使用我这些代码时发现无法产生碰撞，这是意料之内的事情，因为我未开启车辆的碰撞检测
1. 第一步需要修改VehicleFilterShader函数
请修改成例如这样：
  ```cpp
  PxFilterFlags VehicleFilterShader(
	PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	PX_UNUSED(attributes0);
	PX_UNUSED(filterData0);
	PX_UNUSED(attributes1);
	PX_UNUSED(filterData1);
	PX_UNUSED(pairFlags);
	PX_UNUSED(constantBlock);
	PX_UNUSED(constantBlockSize);
	pairFlags = PxPairFlag::eDETECT_DISCRETE_CONTACT | PxPairFlag::eSOLVE_CONTACT;
	return PxFilterFlag::eDEFAULT;
}
  ```
2. 第二步修改initGroundPlane函数中的PxShapeFlag，将第二个PxShapeFlag修改为true，表示开启物理模拟
```cpp
shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
```

3. 第三步修改PhysXIntegration.cpp中的第64行
```cpp
//const PxVehiclePhysXRigidActorShapeParams physxActorShapeParams(boxGeom, physxParams.physxActorBoxShapeLocalPose, defaultMaterial, PxShapeFlags(0), PxFilterData(), 
const PxVehiclePhysXRigidActorShapeParams physxActorShapeParams(boxGeom, physxParams.physxActorBoxShapeLocalPose, defaultMaterial, PxShapeFlags(PxShapeFlag::eSIMULATION_SHAPE),
```

至此，你就会发现碰撞变得有效了。

## Finally
如果有任何疑问，请联系email：2636427505@qq.com
我会尽最大能力帮助你！
