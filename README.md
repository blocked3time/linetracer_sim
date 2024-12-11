# lintracer_sim
opencv라이브 러리를 사용한 라인 검출 알고리즘 

블록도

![image](https://github.com/user-attachments/assets/88ce19b6-a444-4b1c-b1c0-bcb4363ccdb0)


모터 시뮬레이션 유튜브 링크 

![image](https://github.com/user-attachments/assets/191df94f-88ea-481e-88fa-a5b7655dd1de)


https://www.youtube.com/watch?v=RZYVFkru_ac&ab_channel=%EC%A0%95%EC%9A%A9%EA%B7%9C

https://www.youtube.com/watch?v=g123CKNEtks&ab_channel=%EC%A0%95%EC%9A%A9%EA%B7%9C


헤더파일 :

![image](https://github.com/user-attachments/assets/ab66812a-bdbe-46e9-878d-90107cc8685d)


코드 설명 : 
main : 

![image](https://github.com/user-attachments/assets/b75787ec-2b64-455c-a204-6800cdc58819)


필요한 라이브러리 및 using namespace,control + c 함수 , 자주 사용하는 상수를 디파인 해준다.
MINDISTANCE : 라인을 검출 할 때 이전 프레임에서 75픽셀 안에서 라인을 검출한 경우 라인으로 인식, ERR : 에러 보정값, RPM : 모터의 RPM값

 ![image](https://github.com/user-attachments/assets/21343838-a86c-4e22-9e35-f7202cc951b6)

필요한 변수, 객체 선언 및 VideoCapture,VideoWriter 객체를 선언한다. VideoWriter객체는 dstX 의 포트로 영상을 전송하거나 저장한다.

![image](https://github.com/user-attachments/assets/8dfe3056-9bc5-4fba-9299-f2abe7fc46f2)


함수 반복문 동영상이 끝나거나 control + c를 누를 때까지 반복한다.

suorce 로부터 영상 frame을 얻은 후 원본 파일을 writer2로 전송한다.

setFrame 프레임을 자른 후 함수는 밝기처리, gray영상으로 변환,이진화를 수행한다.

시간을 측정하여 출력하고 30프레임을 유지한다

's' 를 입력하면 모터가 입력된 시뮬레이션 영상에 따라 동작한다

![image](https://github.com/user-attachments/assets/f231b193-e183-4983-a51e-f80583383549)


connectedComponentsWithStats 함수를 통해 이미지의 통계 정보를 받는다 각 lables : 구성요소(코드에서 쓰이지 않음), stats : 출력으로 x,y위치 밑 width, heigh, 면적의 정보가 저장됨, centroids : 무게 중심

Point 객체인 po는 이전 프레임에서의 인식된 라인의 좌표를 저장한다. 라인트레이서가 처음 동작하는 경우 라인 중심에서 시작됨으로 포인트의 값을 프레임의 중심으로 설정한다.

findMinIndex함수는 connectedComponentsWithStats를 통해 찾은 이미지 정보에서 이전 프레임에서 가장 가까운 객체의 중심을 찾는다. 가장 작은 객채로 찾은 객체의 거리가 MINDISTANCE보다 큰경우 다른 라인으로 간주한다.
반복문에서 라인을 찾지 못 한 경우(포인트 객체의 값이 바뀌지 않거나 MINDISTANCE보다 작은 거리를 가진 객체가 없는 경우) 포인트 po의 값을 업데이트 하지 않는다.(이전 값 유지)

![image](https://github.com/user-attachments/assets/ef59bf49-1b04-45d8-b732-2357c9a61e2d)

drawBoundingBox함수를 사용하여 바운딩 박스와 무게 중심을 그린다. 이전에 frame을 gray타입으로 변경 했음으로 BFR로 변경한다. findMinIndex로 찾은 index를 매개 변수로 받는다. 객체의 면적이 50 미만인 경우 노이즈로 간주한다.
빨강 : 현재 따라가는 라인,  파랑 : 다른 라인, 노랑 : 노이즈

![image](https://github.com/user-attachments/assets/da9c5c77-8430-4076-a331-5d9975c6a8d0)


getErr함수로 err 값을 받는다. err 값을 RPM에 더하는 방식으로 lvel,rvel 값을 구한다.

![image](https://github.com/user-attachments/assets/0f54f992-c86f-4b83-9a38-26d7c95a2bc2)


이후 처리된 이미지를 저장 및 전송한 후 측정된 err 값, lvel, rvel, 측정 시간의 정보(TickMeter 객체 사용)를 약 0.03 Sec(30fps) 주기로 출력한다. control + c를 눌러 종료할 수 있다.

Makefile

![image](https://github.com/user-attachments/assets/f958eca2-5bb7-41ca-bef7-da9191b814f9)



 

