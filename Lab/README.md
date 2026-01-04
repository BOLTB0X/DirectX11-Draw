# Lab

DriectX 11 개발 연습 및 학습물들 정리

## Intro

<details>
<summary> DirectX 11 란? </summary>

<br/>

### DirectX 11

<p align="center">
   <img src="https://i0.wp.com/howtocheckversion.com/wp-content/uploads/2023/09/1.png?w=800&ssl=1" alt="Example Image" width="100%">
</p>

 **MS가 제공하는 저수준 그래픽 및 멀티미디어 API(Application Programming Interface)**

 - 컴퓨팅 셰이더 (DirectCompute)
  
    *렌더링뿐만 아니라 범용 연산(GPGPU)에도 GPU의 병렬 처리 능력을 활용할 수 있게 되어, 물리 시뮬레이션, AI 연산 등 다양한 분야에 응용 가능*

- 렌더링 성능 및 효율 향상

- GPU 메모리 추상화

- 멀티 스레딩 지원 강화

    Direct3D 11부터 멀티 스레딩을 통한 렌더링 파이프라인 효율 증대


</details>

## 참고

- [reddit - DX11으로 바꾸면 실제로 뭐가 달라지는 거야?](https://www.reddit.com/r/Guildwars2/comments/psd9a0/what_does_change_to_dx11_actually_do/?tl=ko)

- [티스토리 참고 - [알콜코더의 미리 배워보는DX11 입문편] DirectX 11의 특징들(Visual Studio 2010 공식 팀 블로그 @vsts2010:티스토리)](https://vsts2010.tistory.com/510#:~:text=Direct3D%2011%EC%9D%80%20%EC%9C%88%EB%8F%84%EC%9A%B0%20%EB%B9%84%EC%8A%A4%ED%83%80%2C%20%EC%9C%88%EB%8F%84%EC%9A%B0%207%EC%97%90%EC%84%9C%20%EC%A7%80%EC%9B%90%EB%90%98%EB%8A%94,%EB%B3%B4%EB%8B%A4%20%EC%9C%A0%EC%97%B0%ED%95%98%EA%B3%A0%20%EB%9B%B0%EC%96%B4%EB%82%9C%20%EA%B8%B0%EB%8A%A5%EB%93%A4%EC%9D%B4%20%EC%8B%AC%ED%94%8C%ED%95%9C%20API%EC%85%8B%ED%8A%B8%EB%A1%9C%20%EC%A0%9C%EA%B3%B5%EB%90%A9%EB%8B%88%EB%8B%A4.)