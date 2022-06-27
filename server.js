if (process.env.NODE_ENV !== 'production') {
    require('dotenv').config();
}
const express = require('express')
const app = express()
const mongoose = require('mongoose')
const expressLayouts = require ('express-ejs-layouts')
const methodOverride = require('method-override')
const passport = require('passport')
const path = require('path')
const port = 8000;

var resources = path.join(__dirname+ '/resources')
var public = path.join(__dirname+ '/public')
app.use(express.static(resources))
app.use(express.static(public))

app.engine('html', require('ejs').renderFile);
app.set('view engine', 'html');
app.use(expressLayouts)
app.set('layout','layouts/layout.html')

app.use(express.urlencoded({ extended: false }))
app.use(express.json())
app.use(methodOverride('_method'))

mongoose.connect(process.env.DATABASE_URL, {
    useNewUrlParser : true,
    useUnifiedTopology : true
})
const db = mongoose.connection
db.on('error', (error) => console.error(error))
db.once('open', () => console.log('Connected to Database'))

console.log('Server start/restart')
console.log('listening to port',port)
console.log()

app.get('/',(req,res) => {
    res.render('index.html')
    console.log('Viewing Dashboard')
})

const routes_admin = require('./routes/routes_admin');
app.use('/admin',routes_admin)

app.listen(port)