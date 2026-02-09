# Graphics-Draw

![어어어어엇!!](https://github.com/BOLTB0X/Graphics-Draw/blob/main/DemoGIF/%EB%98%A5%EA%BC%AC%EC%87%BC.gif?raw=true)


~~가성비 노후된 노트북으로~~ 그려그려


### Development Environment

- **OS** : Windows 10
- **IDE** : [VisualStudio 2022 community](https://www.microsoft.com/en-us/download/details.aspx?id=17431)
- **Lang** : C++, HLSL
- **Graphics API** : [DirectX 11](https://www.microsoft.com/en-us/download/details.aspx?id=17431)
- **Build & Package Manager** : [vcpkg](https://github.com/microsoft/vcpkg?tab=readme-ov-file)
- **External Libraries**
    - [stb_image](https://github.com/nothings/stb/tree/master) : `jpg`, `png` 파일 로딩
    - [DirectXTK](https://github.com/microsoft/DirectXTK) : `dds` 파일 로딩
    - [assimp](https://github.com/assimp/assimp) : `.obj`, `.fbx` 모델 로딩
    - [Imgui](https://github.com/ocornut/imgui) : 디버깅 및 개발용 UI

---

### Study

- [과정에 생겼던 에러 및 궁금증](https://github.com/BOLTB0X/DirectX11-Draw/blob/main/Lab/Studies/Error-history.md)

- [Noise](https://github.com/BOLTB0X/DirectX11-Draw/blob/main/Lab/Studies/Noise.md)

- [FBM](https://github.com/BOLTB0X/DirectX11-Draw/blob/main/Lab/Studies/FBM.md)

- [RayMarch](https://github.com/BOLTB0X/DirectX11-Draw/blob/main/Lab/Studies/Raymarch.md)

- [Lighting](https://github.com/BOLTB0X/DirectX11-Draw/blob/main/Lab/Studies/Light.md)


## Sisyphus Engine : [구름과 태양](https://github.com/BOLTB0X/DirectX11-Draw/tree/Sun-and-Cloud/SisyphusEngine)

<p align="center">
  <table style="width:100%; text-align:center; border-spacing:20px;">
    <tr>
        <td style="text-align:center; vertical-align:middle;">
        <p align="center">
        <img src="https://github.com/BOLTB0X/Graphics-Draw/blob/main/DemoGIF/volumetric-raymarching/%EB%B3%BC%EB%A5%A8%EB%A7%88%EC%B9%AD06_%EA%B5%AC%EB%A6%84%EC%A7%80%EB%8C%80%EC%99%80%ED%83%9C%EC%96%912.gif?raw=true" 
             alt="image 2" 
             style="; object-fit:contain; border:1px solid #ddd; border-radius:4px;"/>
        </p>
      </td>
    </tr>
    <tr>
      <td style="text-align:center; font-size:14px; font-weight:bold;">
      <p align="center">
      <a></a>노이즈 텍스처 기반 SDF 레이마칭을 이용한 절차적 Volumetric Cloud 렌더링</a>
      </p>
      </td>
    </tr>
  </table>
</p>

순수 코드로 무엇을 렌더링해볼까 고민하다, 구름의 질감을 표현하는게 상당히 어렵다는 것을 알게 되었
음

이 [보랏빛 하늘](https://www.shutterstock.com/ko/image-photo/purple-sky-clouds-backdrop-orange-pink-2678999665?trackingId=19e277b6-eb35-471f-b876-09a679b367e4&listId=searchResults) 사진과 구름과 태양을 렌더링을 도전

- [feature](https://github.com/BOLTB0X/DirectX11-Draw/tree/Sun-and-Cloud/SisyphusEngine#feature): **Volumetric Cloud System** , **Atmospheric Lighting**

-  [자세한 README는 여기 클릭](https://github.com/BOLTB0X/DirectX11-Draw/tree/Sun-and-Cloud/SisyphusEngine#feature) 

<details>
<summary> more 영상 </summary>

<p align="center">
  <table style="width:100%; text-align:center; border-spacing:20px;">
    <tr>
        <td style="text-align:center; vertical-align:middle;">
        <p align="center">
        <img src="https://github.com/BOLTB0X/Graphics-Draw/blob/main/DemoGIF/volumetric-raymarching/%EB%B3%BC%EB%A5%A8%EB%A7%88%EC%B9%AD05_%EA%B5%AC%EB%A6%84%EA%B3%BC%ED%83%9C%EC%96%91.gif?raw=true" 
             alt="image 2" 
             style="width:600px; height:400px; object-fit:contain; border:1px solid #ddd; border-radius:4px;"/>
        </p>
      </td>
      <td style="text-align:center; vertical-align:middle;">
        <p align="center">
        <img src="https://github.com/BOLTB0X/Graphics-Draw/blob/main/DemoGIF/volumetric-raymarching/%EB%B3%BC%EB%A5%A8%EB%A7%88%EC%B9%AD05_%EA%B5%AC%EB%A6%84%EA%B3%BC%ED%83%9C%EC%96%912.gif?raw=true" 
             alt="image 2" 
             style="width:600px; height:400px; object-fit:contain; border:1px solid #ddd; border-radius:4px;"/>
        </p>
      </td>
    </tr>
    <tr>
      <td style="text-align:center; font-size:14px; font-weight:bold;">
      <p align="center">
      <a></a>구름 하나</a>
      </p>
      </td>
      <td style="text-align:center; font-size:14px; font-weight:bold;">
      <p align="center">
      <a></a>구름 정면</a>
      </p>
      </td>
    </tr>
  </table>
</p>


<p align="center">
  <table style="width:100%; text-align:center; border-spacing:20px;">
    <tr>
        <td style="text-align:center; vertical-align:middle;">
        <p align="center">
        <img src="https://github.com/BOLTB0X/Graphics-Draw/blob/main/DemoGIF/volumetric-raymarching/%EB%B3%BC%EB%A5%A8%EB%A7%88%EC%B9%AD06_%EA%B5%AC%EB%A6%84%EC%A7%80%EB%8C%80%EC%99%80%ED%83%9C%EC%96%913.gif?raw=true" 
             alt="image 2" 
             style="width:600px; height:400px; object-fit:contain; border:1px solid #ddd; border-radius:4px;"/>
        </p>
      </td>
      <td style="text-align:center; vertical-align:middle;">
        <p align="center">
        <img src="https://github.com/BOLTB0X/Graphics-Draw/blob/main/DemoGIF/volumetric-raymarching/%EB%B3%BC%EB%A5%A8%EB%A7%88%EC%B9%AD06_%EA%B5%AC%EB%A6%84%EC%A7%80%EB%8C%80%EC%99%80%ED%83%9C%EC%96%914%EC%9D%B4%EB%8F%99.gif?raw=true" 
             alt="image 2" 
             style="width:600px; height:400px; object-fit:contain; border:1px solid #ddd; border-radius:4px;"/>
        </p>
      </td>
    </tr>
    <tr>
      <td style="text-align:center; font-size:14px; font-weight:bold;">
      <p align="center">
      <a></a>구름 지대 1</a>
      </p>
      </td>
      <td style="text-align:center; font-size:14px; font-weight:bold;">
      <p align="center">
      <a></a>구름 지대 2</a>
      </p>
      </td>
    </tr>
  </table>
</p>

<p align="center">
  <table style="width:100%; text-align:center; border-spacing:20px;">
      <td style="text-align:center; vertical-align:middle;">
        <p align="center">
        <img src="https://github.com/BOLTB0X/DirectX11-Draw/blob/main/DemoGIF/volumetric-raymarching/%EB%B3%BC%EB%A5%A8%EB%A7%88%EC%B9%AD06_%EA%B5%AC%EB%A6%84%EC%A7%80%EB%8C%80%EC%99%80%ED%83%9C%EC%96%915%20%EB%8C%80%EA%B8%B0%EC%A4%91%20%EC%82%B0%EB%9E%80%20%EB%B0%8F%20%EA%B8%80%EB%A1%9C%EC%9A%B0%20%ED%9A%A8%EA%B3%BC.gif?raw=true" 
             alt="image 2" 
             style="width:600px; height:400px; object-fit:contain; border:1px solid #ddd; border-radius:4px;"/>
        </p>
      </td>
      <td style="text-align:center; vertical-align:middle;">
        <p align="center">
        <img src="https://github.com/BOLTB0X/DirectX11-Draw/blob/main/DemoGIF/volumetric-raymarching/%EB%B3%BC%EB%A5%A8%EB%A7%88%EC%B9%AD07_%EB%A0%8C%EC%A6%88%ED%94%8C%EB%A0%88%EC%96%B4%EA%B3%A0%EC%8A%A4%ED%8A%B8.gif?raw=true" 
             alt="image 2" 
             style="width:600px; height:400px; object-fit:contain; border:1px solid #ddd; border-radius:4px;"/>
        </p>
      </td>
    </tr>
    <tr>
      <td style="text-align:center; font-size:14px; font-weight:bold;">
      <p align="center">
      <a></a>대기 중 햇 빛산란</a>
      </p>
      </td>
      <td style="text-align:center; font-size:14px; font-weight:bold;">
      <p align="center">
      <a></a>렌즈플레어(고스트)</a>
      </p>
      </td>
    </tr>
  </table>
</p>

- [시행착오 및 기록물들](https://github.com/BOLTB0X/DirectX11-Draw/tree/main/DemoGIF/volumetric-raymarching)

- [자세한 `src` 폴더 구조 및 코드 설명](https://github.com/BOLTB0X/DirectX11-Draw/tree/Sun-and-Cloud/SisyphusEngine#src)

    - [파이프라인 - 그래픽](https://github.com/BOLTB0X/DirectX11-Draw/blob/Sun-and-Cloud/SisyphusEngine/src/RederingEngine/RenderingEngine.cpp)

    - [파이프라인 - etc](https://github.com/BOLTB0X/DirectX11-Draw/blob/Sun-and-Cloud/SisyphusEngine/src/MainEngine/MainEngine.cpp)

    - [Cloud HLSL 코드](https://github.com/BOLTB0X/DirectX11-Draw/blob/Sun-and-Cloud/SisyphusEngine/src/HLSL/CloudPS.hlsl)

    - [Sky HLSL 코드](https://github.com/BOLTB0X/DirectX11-Draw/blob/Sun-and-Cloud/SisyphusEngine/src/HLSL/SkyPS.hlsl)

</details>

<details>
<summary> Ref </summary>

- [maximeheckel - Three.js : Real-time dreamy Cloudscapes with Volumetric Raymarching](https://blog.maximeheckel.com/posts/real-time-cloudscapes-with-volumetric-raymarching/)

- [42yeah - Raymarching Clouds](https://blog.42yeah.is/rendering/2023/02/11/clouds.html)

- [Chris' Graphics Blog - Volumetric Rendering](https://wallisc.github.io/rendering/2020/05/02/Volumetric-Rendering-Part-1.html)

- [h3r3 - shadertoy : Sunset on the sea](https://www.shadertoy.com/view/4dl3zr)

- [Mythical - godotshaders : Cloud material](https://godotshaders.com/shader/cloud-material/)

- [shff - github : OpenGL Sky](https://github.com/shff/opengl_sky)

</details>

## 참고


- [rastertek - DirectX 11 on Windows 10 Tutorials](https://www.rastertek.com/tutdx11win10.html)

- [rastertek - DirectX 11 on Windows 10 Tutorials 2](https://www.rastertek.com/tutdx11s2ter.html)

- [Learn microsoft - Direct3D 11](https://learn.microsoft.com/ko-kr/windows/win32/direct3d11/dx-graphics-overviews)

- [유튜브 - 환경 설정 | 홍정모, 프로그래밍 기초, 입문 프로그래밍, 게임 프로그래밍, 프로그래밍 강의, 컴퓨터 그래픽스, 게임 개발, 컴퓨터 작동원리, 코딩](https://www.youtube.com/watch?v=CcFtxKg55fM&list=PLNfg4W25Tapy3qmKqftWndwrIaQRzUmEg&index=9)

- [stack overflow - Basic example for a CMake project definition using DirectX 11 on Windows 10?](https://stackoverflow.com/questions/73986114/basic-example-for-a-cmake-project-definition-using-directx-11-on-windows-10#:~:text=cmake_minimum_required%20(VERSION%203.13)%20project(CMakeDirect3DGame%20DESCRIPTION%20%22CMake%20example,_WIN32_WINNT=0x0601)%20endif()%20endif()%20set_property(DIRECTORY%20PROPERTY%20VS_STARTUP_PROJECT%20$%7BPROJECT_NAME%7D))

- [티스토리 참고  - DirectX 11 최초 실행 환경 세팅하기(나는 뉴비다 개발자편)](https://dev-nicitis.tistory.com/43)

- [티스토리 참고 - DirectX 11 도형 출력(버텍스 버퍼, 버텍스 쉐이더, 픽셀 쉐이더 설정)(pub-repository)](https://pub-repository.tistory.com/97)

- [티스토리 참고 - [DirectX11] 모델 가져오기(Model Import)(유니얼)](https://unialgames.tistory.com/entry/DirectX11ModelImport)

- [티스토리 참고 - FBX SDK, FBX Importer, FBX 파일 불러오기, 3ds Max 등(최디디)](https://dlemrcnd.tistory.com/85)

- [티스토리 참고 - 070 OBB(Oriented Bounding Box) collision(designerd)](https://designerd.tistory.com/entry/DirectX11-070-OBBOriented-Bounding-Box-collision#obb-%EC%B6%A9%EB%8F%8C---%EB%B6%84%EB%A6%AC%EC%B6%95-%EC%9D%B4%EB%A1%A0)

- [티스토리 참고 - Collision과 SphereCollider(유니얼)](https://unialgames.tistory.com/entry/DirectX11CollisionAndSphereCollider)

- [티스토리 참고 - Terrain 21 - 지형셀(copynull)](https://copynull.tistory.com/324)

- [Velog 참고 - Terrain 지형 툴 연습(hoya1215)](https://velog.io/@hoya1215/Directx11-Terrain-%EC%A7%80%ED%98%95-%ED%88%B4-%EC%97%B0%EC%8A%B5)

- [티스토리 참고 - DirectX11 3D(designerd)](https://designerd.tistory.com/category/%E2%AD%90%20DirectX/DirectX11%20%203D)

- [벨로그 참고 - Imgui 사용법(charlese_note)](https://velog.io/@charlese_note/ImGUI)

- [네이버 블로그 참고 - Dear ImGui를 공부해보자! 시작단계(AROMA)](https://m.blog.naver.com/sweetsmell9/221618574623)

- [티스토리 참고 - Blender AO map Node | 엠비언트 맵 연결하기(lioicreim)](https://lioicreim.tistory.com/615)

- [네이버 블로그 참고 - DirectX 트리 ( 쿼드트리, 옥트리 ) 기본 설명(로기)](https://m.blog.naver.com/PostView.nhn?isHttpsRedirect=true&blogId=jsjhahi&logNo=201309282)

- [티스토리 - 쿼드트리(copynull)](https://copynull.tistory.com/308)


- [노이즈 텍스처 출처 - realtimevfx](https://realtimevfx.com/t/realistic-cloud-texture/19207)

- [블로그 참고 - 불륨마칭(maximeheckel)](https://blog.maximeheckel.com/posts/real-time-cloudscapes-with-volumetric-raymarching/)

- [hree.js forum 참고 - volumetric clouds - game ready](https://discourse.threejs.org/t/volumetric-clouds-game-ready/86598)

- [goto Shader 코드 참고](https://godotshaders.com/shader/cloud-material/)

- [블로그 참고 - 1인칭 카메라 조작(mstone8370)](https://mstone8370.tistory.com/26)

- [github 참고 - Volumetric-Rendering(Chris' Graphics Blog)](https://wallisc.github.io/rendering/2020/05/02/Volumetric-Rendering-Part-1.html)

- [티스토리 참고 - DirectX 11 Tone Mapping(kyuhwang)](https://kyuhwang.tistory.com/36)

#### [모델 출처 -> sketchfab](https://sketchfab.com/feed)

- [Stone 이용: sketchfab - Ground//Stone Sphere(Madee)](https://sketchfab.com/3d-models/groundstone-sphere-1c0f2b2e213348e6a760743a546dc7a6)

- [normalMap 이용: filterforge `Cloud_normal.jpg`](https://www.filterforge.com/filters/6395-normal.html)

- [Mountain 이용: sketchfab - Cloudy Mountain(Pers Scans)](https://sketchfab.com/3d-models/cloudy-mountain-f61d84c6534d4007accdeac35f433119)