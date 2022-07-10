const express = require('express')
const Admin = require('./../models/admin.js')
const router = express.Router()
const bcrypt = require('bcrypt')
const passport = require('passport')
const flash = require('express-flash')
const session = require('express-session')
const methodOverride = require('method-override')
const initializePassport = require('./../passport-config')
const DateOnly = require('dateonly');


router.use(flash())
router.use(session({
    secret : process.env.SESSION_SECRET,
    resave : false,
    saveUninitialized : false
}))
initializePassport(
    passport,
    username => Admin.findOne(user => user.username === username),
  	id => Admin.findOne(user => user.id === id)
)

router.use(passport.initialize())
router.use(passport.session())
router.use(methodOverride('_method'))

router.get('/',checkAunthenticated,(req, res) => {
	console.log('Viewing admin panel')
	res.render('../views/admin/admin.html')
})

router.get('/login',checkNotAunthenticated,(req, res) => {
	console.log('Viewing Login Page')
	res.render('../views/admin/login.html')
})

router.post('/login',checkNotAunthenticated, passport.authenticate('local',{
	successRedirect : '/admin',
	failureRedirect : '/admin/login',
	failureFlash : true
}))

router.get('/register',checkAunthenticated, (req, res) => {
	console.log('Viewing admin register panel')
	res.render('../views/admin/register.html')
})

router.post('/register',checkAunthenticated, async(req,res,next) => {
	try{
	const cryptedPassword = await bcrypt.hash(req.body.password, 10)
	let admin = new Admin({
			id : Date.now().toString(),
			username : req.body.username,
			// password : req.body.password
			password : cryptedPassword
		})
		admin = admin.save()
		console.log('Register Post')
		next()
		res.redirect('/admin')
	}
	catch (e){
		console.log(e)
		res.redirect('/admin/success')
	}
})

router.get('/change_password',checkAunthenticated, (req, res) => {
	console.log('Viewing admin change password panel')
	res.render('../views/admin/change_password.html')
})

router.patch('/change_password',checkAunthenticated, async(req,res,next) => {
	let search
	try{
		search = Admin.findOne({ username: req.body.username }, async(e, user) =>{
            if (e) return (e);
            if (!user) {
				console.log('No Admin With that Username')
				next();
				res.redirect('/admin/change_password')
				return { message: 'Incorrect username / password.' }
			}
			else{
				try{
					const UpdatedPassword = await bcrypt.hash(req.body.password, 10)
					await Admin.updateOne(
						{username : req.body.username},
						{password : UpdatedPassword}
					)
					console.log('password changed')
					next()
					res.redirect('/admin')
				}catch (e){
					console.log('change password failed')
					console.log(e)
					res.redirect('/admin/change_password')
				}
			}
            });
    }catch (e){
		console.log(e)
		res.redirect('/admin/change_password')
		}
})

router.delete('/logout', (req, res) => {
	console.log('logout')
	req.logOut()
	res.redirect('/')
  })

function checkAunthenticated(req, res, next) {
	if (req.isAuthenticated()) {
	  return next()
	}
	res.redirect('/admin/login')

	// if (req.isAuthenticated()) {
	// 	return res.redirect('/admin')
	//   }
	//   next()
  }
  
function checkNotAunthenticated(req, res, next) {
	
	if (req.isAuthenticated()) {
	  return res.redirect('/admin')
	}
	next()
  }

module.exports = router