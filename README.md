# DirectX 11 - Draw

![어어어어엇!!](https://github.com/BOLTB0X/Graphics-Draw/blob/main/DemoGIF/%EB%98%A5%EA%BC%AC%EC%87%BC.gif?raw=true)


~~가성비 노후된 노트북으로~~ **Sisyphus Engine** 으로 그려그려


## Sisyphus Engine: Tech Stack

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

## [Sisyphus Engine : 구름과 태양](https://github.com/BOLTB0X/DirectX11-Draw/tree/Sun-and-Cloud/SisyphusEngine)

<p align="center">
  <table style="width:100%; text-align:center; border-spacing:20px;">
    <tr>
        <td style="text-align:center; vertical-align:middle;">
        <p align="center">
        <img src="https://github.com/BOLTB0X/DirectX11-Draw/blob/main/DemoGIF/volumetric-raymarching/%EA%B5%AC%EB%A6%84%EA%B3%BC%ED%83%9C%EC%96%91.png?raw=true" 
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

- [feature](https://github.com/BOLTB0X/DirectX11-Draw/tree/Sun-and-Cloud/SisyphusEngine#feature): **Ray Marching**, **Volumetric Cloud System** , **Atmospheric Lighting**

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

---

## [Sisyphus Engine : 렌즈플레어](https://github.com/BOLTB0X/DirectX11-Draw/tree/LensFlare/SisyphusEngine)

<p align="center">
  <table style="width:100%; text-align:center; border-spacing:20px;">
    <tr>
        <td style="text-align:center; vertical-align:middle;">
        <p align="center">
        <img src="https://github.com/BOLTB0X/DirectX11-Draw/blob/main/DemoGIF/lensFlare/%EB%A0%8C%EC%A6%88%ED%94%8C%EB%A0%88%EC%96%B4.png?raw=true" 
             alt="image 2" 
             style="; object-fit:contain; border:1px solid #ddd; border-radius:4px;"/>
        </p>
      </td>
        <td style="text-align:center; vertical-align:middle;">
        <p align="center">
        <img src="https://github.com/BOLTB0X/DirectX11-Draw/blob/main/DemoGIF/lensFlare/%EB%A0%8C%EC%A6%88%ED%94%8C%EB%A0%88%EC%96%B407_%EC%A0%88%EC%B0%A8%EC%A0%8100.png?raw=true" 
             alt="image 2" 
             style="; object-fit:contain; border:1px solid #ddd; border-radius:4px;"/>
        </p>
      </td>
    </tr>
    <tr>
      <td style="text-align:center; font-size:14px; font-weight:bold;">
      <p align="center">
      <a></a>texture LensFlare</a>
      </p>
      </td>
            <td style="text-align:center; font-size:14px; font-weight:bold;">
      <p align="center">
      <a></a>noise LensFlare</a>
      </p>
      </td>
    </tr>
  </table>
</p>

렌즈 플레어 고정된 현상을 해결을 위해,

[John Chapman의 Pseudo LensFlare](https://john-chapman-graphics.blogspot.com/2013/02/pseudo-lens-flare.html) 과 [shadertoy - musk's lens flare mod(icecool)](https://www.shadertoy.com/view/XdfXRX) 을 기반으로 렌즈플레어 적용

- [feature](https://github.com/BOLTB0X/DirectX11-Draw/tree/LensFlare/SisyphusEngine#feature): **Ghost-Halo-Glow Generation** , **Falloff** , **Visibility Check** , **LensDrift**

-  [자세한 README는 여기 클릭](https://github.com/BOLTB0X/DirectX11-Draw/tree/LensFlare/SisyphusEngine#feature) 

<details>
<summary> more 영상 </summary>

<p align="center">
  <table style="width:100%; text-align:center; border-spacing:20px;">
      <td style="text-align:center; vertical-align:middle;">
        <p align="center">
        <img src="https://github.com/BOLTB0X/DirectX11-Draw/blob/main/DemoGIF/lensFlare/%EB%A0%8C%EC%A6%88%ED%94%8C%EB%A0%88%EC%96%B404_Distorted02.png?raw=true" 
             alt="image 2" 
             style="width:600px; height:400px; object-fit:contain; border:1px solid #ddd; border-radius:4px;"/>
        </p>
      </td>
      <td style="text-align:center; vertical-align:middle;">
        <p align="center">
        <img src="https://github.com/BOLTB0X/DirectX11-Draw/blob/main/DemoGIF/lensFlare/%EB%A0%8C%EC%A6%88%ED%94%8C%EB%A0%88%EC%96%B406_drift02.gif?raw=true" 
             alt="image 2" 
             style="width:600px; height:400px; object-fit:contain; border:1px solid #ddd; border-radius:4px;"/>
        </p>
      </td>
    </tr>
    <tr>
      <td style="text-align:center; font-size:14px; font-weight:bold;">
      <p align="center">
      <a></a>Texture 기반(정면)</a>
      </p>
      </td>
      <td style="text-align:center; font-size:14px; font-weight:bold;">
      <p align="center">
      <a></a>Texture 기반(Drift)</a>
      </p>
      </td>
    </tr>
  </table>
</p>

<p align="center">
  <table style="width:100%; text-align:center; border-spacing:20px;">
      <td style="text-align:center; vertical-align:middle;">
        <p align="center">
        <img src="https://github.com/BOLTB0X/DirectX11-Draw/blob/main/DemoGIF/lensFlare/%EB%A0%8C%EC%A6%88%ED%94%8C%EB%A0%88%EC%96%B407_%EC%A0%88%EC%B0%A8%EC%A0%8102.gif?raw=true" 
             alt="image 2" 
             style="width:600px; height:400px; object-fit:contain; border:1px solid #ddd; border-radius:4px;"/>
        </p>
      </td>
      <td style="text-align:center; vertical-align:middle;">
        <p align="center">
        <img src="https://github.com/BOLTB0X/DirectX11-Draw/blob/main/DemoGIF/lensFlare/%EB%A0%8C%EC%A6%88%ED%94%8C%EB%A0%88%EC%96%B407_%EC%A0%88%EC%B0%A8%EC%A0%8104_%EC%8B%9C%EC%95%BC.gif?raw=true" 
             alt="image 2" 
             style="width:600px; height:400px; object-fit:contain; border:1px solid #ddd; border-radius:4px;"/>
        </p>
      </td>
    </tr>
    <tr>
      <td style="text-align:center; font-size:14px; font-weight:bold;">
      <p align="center">
      <a></a>Noise 기반 procedural(Drift)</a>
      </p>
      </td>
      <td style="text-align:center; font-size:14px; font-weight:bold;">
      <p align="center">
      <a></a>Noise 기반 procedural(vision)</a>
      </p>
      </td>
    </tr>
  </table>
</p>

- [시행착오 및 기록물들](https://github.com/BOLTB0X/DirectX11-Draw/tree/main/DemoGIF/lensFlare)

- [자세한 `src` 폴더 구조 및 코드 설명](https://github.com/BOLTB0X/DirectX11-Draw/tree/LensFlare/SisyphusEngine/src)

    - [파이프라인 - 그래픽](https://github.com/BOLTB0X/DirectX11-Draw/blob/LensFlare/SisyphusEngine/src/RederingEngine/RenderingEngine.cpp)

    - [파이프라인 - etc](https://github.com/BOLTB0X/DirectX11-Draw/blob/LensFlare/SisyphusEngine/src/MainEngine/MainEngine.cpp)

    - [LensFlare Pixel Shader](https://github.com/BOLTB0X/DirectX11-Draw/blob/LensFlare/SisyphusEngine/src/HLSL/LensFlarePS.hlsl)

    - [셰이더 버퍼 정의(C++)](https://github.com/BOLTB0X/DirectX11-Draw/blob/LensFlare/SisyphusEngine/src/RederingEngine/Shader/ShaderBuffers.h)

</details>


<details>
<summary> Ref </summary>

- [John Chapman Graphics 블로그 - Pseudo LensFlare](https://john-chapman-graphics.blogspot.com/2013/02/pseudo-lens-flare.html)

- [ShaderToy - musk's lens flare mod(Icecool)](https://www.shadertoy.com/view/XdfXRX)

- [therealmjp - Stairway To (Programmable Sample Point) Heaven](https://therealmjp.github.io/posts/programmable-sample-points/)

- [pngtree - 이미지 사용](https://pngtree.com/so/lens-flare)

</details>

---

## [Lab : 연습 및 흔적들 정리](https://github.com/BOLTB0X/DirectX11-Draw/tree/main/Lab)

<details>
<summary> Issue </summary>

### Issue

- [포스트 프로세싱 SRV 에러](https://github.com/BOLTB0X/DirectX11-Draw/issues/4) -> [포스트 프로세싱이 잘못된걸 고치는 이야기](https://github.com/BOLTB0X/DirectX11-Draw/blob/main/Lab/Studies/BicubicModifyingPlaneScene.md)

- [렌즈플레어 적용시 화면 고스트 고정 현상](https://github.com/BOLTB0X/DirectX11-Draw/issues/5) -> [Ghost가 고정되었던 이유에서 존 챔피언 방식으로 고치는 이야기](https://github.com/BOLTB0X/DirectX11-Draw/blob/main/Lab/Studies/FixedGhost.md)

</details>

<details>
<summary> Study </summary>

### Study

- [DX11 기초 지식](https://github.com/BOLTB0X/DirectX11-Draw/tree/main/Lab#%EC%95%8C%EB%A9%B4-%EC%A2%8B%EC%9D%80-%EA%B2%83)

- [Noise](https://github.com/BOLTB0X/DirectX11-Draw/blob/main/Lab/Studies/Noise.md)

- [FBM](https://github.com/BOLTB0X/DirectX11-Draw/blob/main/Lab/Studies/FBM.md)

- [RayMarch](https://github.com/BOLTB0X/DirectX11-Draw/blob/main/Lab/Studies/Raymarch.md)

- [Lighting](https://github.com/BOLTB0X/DirectX11-Draw/blob/main/Lab/Studies/Light.md)


</details>

---