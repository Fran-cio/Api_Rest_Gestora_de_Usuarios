<!-- Improved compatibility of back to top link: See: https://github.com/othneildrew/Best-README-Template/pull/73 -->
<a name="readme-top"></a>
<!--
*** Thanks for checking out the Best-README-Template. If you have a suggestion
*** that would make this better, please fork the repo and create a pull request
*** or simply open an issue with the tag "enhancement".
*** Don't forget to give the project a star!
*** Thanks again! Now go create something AMAZING! :D
-->



<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]



<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/Fran-cio/restApiUserManager">
    <img src="images/logo.png" alt="Logo" width="250" height="250">
  </a>

<h3 align="center">project_title</h3>

  <p align="center">
  Basic implementation of a RESTful API on an embedded system
    <br />
    <a href="https://github.com/Fran-cio/restApiUserManager"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/Fran-cio/restApiUserManager">View Demo</a>
    ·
    <a href="https://github.com/Fran-cio/restApiUserManager/issues">Report Bug</a>
    ·
    <a href="https://github.com/Fran-cio/restApiUserManager/issues">Request Feature</a>
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#installation">Installation</a></li>
        <li><a href="#uninstallation">Uninstallation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project
<br>
Embedded systems are often accessed remotely. There are different techniques to do this, with one commonly used method being RESTful APIs. These provide a defined and robust interface for remote communication and manipulation of the embedded system.
<p align="right">(<a href="#readme-top">back to top</a>)</p>



### Built With

* ![Linux][Linux]
* ![C][C]
* [![Cmake][Cmake]][Cmake-url]
* [![Neovim][Neovim]][Neovim-url]
* [![Nginx][Nginx]][Nginx-url]

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started
If any of the mentioned prerequisites already have a service running with those applications, it is necessary to manually verify the operations performed to avoid formatting issues in the configuration files.
### Prerequisites
Access the installation instructions for the following dependencies.
* [Nginx](https://nginx.com/) 
* [Ulfius](https://github.com/babelouest/ulfius/tree/master) 
* [Yder](https://github.com/babelouest/yder)
* [Jansson](https://github.com/akheron/jansson) 

### Installation

1. Clone the repo
   ```sh
   git clone https://github.com/Fran-cio/restApiUserManager.git
   ```
2. Make the project 
   ```sh
   make install
   ```
### Uninstallation

1. Use Make clean
    ```sh
    make remove
    ```
<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- USAGE EXAMPLES -->
## Usage
Once the installation has been successfully executed, the user management service is installed correctly, and it is possible to point to the API's POST endpoints and use the service.


Next, we will detail the two services to be created and their functionalities.

### User Service
This service will be responsible for creating users and listing them. These users should be able to log in via SSH after their creation.

#### POST /api/users
Endpoints for user creation in the operating system:

```C
    POST http://{{server}}/api/users
```
Request
```C    
    curl --request POST \
        --url http://{{server}}/api/users \
        -u USER:SECRET \
        --header 'accept: application/json' \
        --header 'content-type: application/json' \
        --data '{"username": "myuser", "password": "mypassword"}'
```
Response
```C
    {
        "id": 142,
        "username": "myuser",
        "created_at": "2019-06-22 02:19:59"
    }
```
The log message will be: _User \<Id\> created_.

Each time a user is created, this service must increment the counter in the user counter service. If the counter service is not available or fails, log the failure.

#### GET /api/users
Endpoint to retrieve all users of the operating system and their identifiers.
```C
    GET http://{{server}}/api/users
```
Request
```C
    curl --request GET \
        --url http://{{server}}/api/users \
        -u USER:SECRET \
        --header 'accept: application/json' \
        --header 'content-type: application/json'
```
Response
```C
    {
        "data": [
            {
                "user_id": 2,
                "username": "user1",  
            },
            {
                "user_id": 1,
                "username": "user2"
            },
            ...
        ]
    }
```
The log message will be: _Users created: \<number of users in the OS\>_

### Counter Service
This microservice has the endpoints from Lab 5, it maintains a counter. This counter must be implemented with the same Media Type, `application/json`.

#### POST /counter/increment
```C
    POST http://{{server}}/counter/increment
```
Request

```C
    curl --request POST \
        --url http://{{server}}/counter/increment \
        -u USER:SECRET \
        --header 'accept: application/json' \
        --header 'content-type: application/json'
```

Response
```C
    {
        "code": 200,
        "description": "<new_value>"
    }
```

The log message will be: _Counter incremented from: \<Client IP that incremented the counter\>_


#### GET /counter/value
This endpoint allows you to know the current value of the counter.
```C
    GET http://{{server}}/counter/value
```
Request
```C
    curl --request GET \
        --url http://{{server}}/counter/value \
        -u USER:SECRET \
        --header 'accept: application/json' \
        --header 'content-type: application/json'
```
Response

```C
    {
        "code": 200,
        "description": "<value>"
    }
```
Additionally, we have the reverse proxy service of *Nginx* and the ability to log all activity with the *logrotate* service.

For user creation, it's possible to use the [`crear_usuario.sh`](crear_usuario-script) script, which allows creating users through a simple user interface.
<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- LICENSE -->
## License

Distributed under the Apache License 2.0. See `LICENSE.txt` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CONTACT -->
## Contact

Francisco Ciordia Cantarella - francisco.ciordia.cantarella@gmail.com

Project Link: [https://github.com/Fran-Cio/restApiUserManager](https://github.com/Fran-Cio/restApiUserManager)

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

* [Pretty Badges](https://github.com/Ileriayo/markdown-badges)
* [Logo Generator](https://www.bing.com/images/create?FORM=BICMB1&ssp=1&darkschemeovr=0&setlang=es-CL&safesearch=moderate&toWww=1&redig=8195C3604CE2443CAD2B9B2FB3512087)
* [Template of this readme](https://github.com/othneildrew/Best-README-Template?tab=readme-ov-file)
* [Api Rest framework](https://github.com/babelouest/ulfius/tree/master) 
* [Logger for C](https://github.com/babelouest/yder)
* [Json parser in C](https://github.com/akheron/jansson) 
<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/Fran-Cio/restApiUserManager.svg?style=for-the-badge
[contributors-url]: https://github.com/Fran-cio/restApiUserManager/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/Fran-cio/restApiUserManager.svg?style=for-the-badge
[forks-url]: https://github.com/Fran-Cio/restApiUserManager/network/members
[stars-shield]: https://img.shields.io/github/stars/Fran-Cio/restApiUserManager.svg?style=for-the-badge
[stars-url]: https://github.com/Fran-Cio/restApiUserManager/stargazers
[issues-shield]: https://img.shields.io/github/issues/Fran-Cio/restApiUserManager.svg?style=for-the-badge
[issues-url]: https://github.com/Fran-Cio/restApiUserManager/issues
[license-shield]: https://img.shields.io/github/license/Fran-cio/Api_Rest_Gestora_de_Usuarios?style=for-the-badge
[license-url]: https://github.com/Fran-Cio/restApiUserManager/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://www.linkedin.com/in/francisco-ciordia-cantarella-5323461b8/
[product-screenshot]: images/screenshot.png
[usage-example]: images/example.png
[crear_usuario-script]: ./crear_usuario.sh

[C]:https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white
[CMake]:https://img.shields.io/badge/CMake-%23008FBA.svg?style=for-the-badge&logo=cmake&logoColor=white
[Cmake-url]:https://cmake.org/
[Nginx]:https://img.shields.io/badge/nginx-%23009639.svg?style=for-the-badge&logo=nginx&logoColor=white
[Nginx-url]:https://www.nginx.com/

[Neovim]:https://img.shields.io/badge/NeoVim-%2357A143.svg?&style=for-the-badge&logo=neovim&logoColor=white
[Neovim-url]:https://neovim.io/
[Linux]:https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black
