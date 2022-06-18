import React, { Component, useState } from 'react';
import Cookies from 'universal-cookie';
import axios from 'axios';
import 'bootstrap/dist/css/bootstrap.min.css';
import '../css/Menu.css';

const cookies = new Cookies();

class Menu extends Component {
    cerrarSesion=()=>{
        cookies.remove('id', {path: "/"});
        cookies.remove('apellido1', {path: "/"});
        cookies.remove('apellido2', {path: "/"});
        cookies.remove('nombre', {path: "/"});
        cookies.remove('username', {path: "/"});
        window.location.href='./';
    }

    componentDidMount() {
        if(!cookies.get('username')){
            window.location.href="./";
        }
    }

    render() {
        console.log('id: '+ cookies.get('id'));
        console.log('apellido1: '+cookies.get('apellido1'));
        console.log('apellido2: '+cookies.get('apellido2'));
        console.log('nombre: '+cookies.get('nombre'));
        console.log('username: '+cookies.get('username'));


        /*
        const [archivos,setArchivos]=useState(null); 

        const subirArchivos=e=>{
            setArchivos(e);
        }

        const insertarArchivos=async()=>{
            const f = new FormData();

            for (let index = 0; index < archivos.length; index++) {
                f.append("files",archivos[index]);
                
            }
            await axios.post("http://localhost:3000/Menu",f, {headers: {'Content-Type':'multipart/form-data'}})
            .then(response=>{
                console.log(response.data);
            }).catch(error=>{
                console.log(error);
            })
        }*/
        /**onChange={(e)=>subirArchivos(e.target.files)}


        onClick={()=>insertarArchivos()}
        */
        return (
            <div>
                Menu Principal
                <div className='Subida_Archivos'>                
                    <br></br>
                    <input type='file' name='files' multiple/>
                    <br></br>
                    <br></br>
                    <button className='btn btn-primary'>Subir Archivo</button>

                    <button style={{marginLeft: 100}}>Descargar</button>
                </div>


                <br>
                </br>

                <br></br>
                <br></br>
                <button  onClick={()=>this.cerrarSesion()}>Cerrar Sesión</button>
            </div>
        );
    }
}

export default Menu;