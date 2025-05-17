import { initializeApp } from "firebase/app";
import { getDatabase, ref, onValue, set } from "firebase/database";

const firebaseConfig = {
  apiKey: "AIzaSyCztyzTjcDiOZucr8elpd5rY4vodp52r1Y",
  authDomain: "kost-oren-kpri-sekawan.firebaseapp.com",
  databaseURL: "https://kost-oren-kpri-sekawan-default-rtdb.asia-southeast1.firebasedatabase.app",
  projectId: "kost-oren-kpri-sekawan",
  storageBucket: "kost-oren-kpri-sekawan.appspot.com",
  messagingSenderId: "874812411732",
  appId: "1:874812411732:web:888372a7035e1ed02ac2c1",
  measurementId: "G-TVS8VBFQV2"
};

const app = initializeApp(firebaseConfig);
const database = getDatabase(app);

export { database, ref, onValue, set };
