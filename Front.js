import React, { useEffect, useState } from 'react';
import axios from 'axios';

const App = () => {
    const [data, setData] = useState([]);

    useEffect(() => {
        const fetchData = async () => {
            const response = await axios.get('http://localhost:3000/api/data');
            setData(response.data);
        };
        fetchData();
    }, []);

    return (
        <div>
            <h1>Dados do Sensor</h1>
            <ul>
                {data.map((item, index) => (
                    <li key={index}>
                        Temperatura: {item.temperature} °C, Umidade: {item.humidity} %, Velocidade: {item.speed} RPM
                    </li>
                ))}
            </ul>
        </div>
    );
};

export default App;
