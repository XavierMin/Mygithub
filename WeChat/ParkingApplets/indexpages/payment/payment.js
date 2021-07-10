// pages/payment/payment.js
Page({

  /**
   * 页面的初始数据
   */
  data: {
    parkArray: [
      {parkid: '0', parkname: "现代光谷世贸中心停车场"}, 
      {parkid: '1', parkname: "光谷智慧园停车场"},
      {parkid: '2', parkname: "光谷金融港停车场"},
      {parkid: '3', parkname: "光谷汇金中心停车场"},
      {parkid: '4', parkname: "中国太平停车场"},
      {parkid: '5', parkname: "青年公寓停车场"},
      {parkid: '6', parkname: "城市之光停车场"},
      {parkid: '7', parkname: "当代国际花园停车场"},
      {parkid: '8', parkname: "泰康人寿大厦停车场"},
      {parkid: '9', parkname: "创客星停车场"}
    ],
    // 省份简写
    provinces: [
      ['京', '沪', '粤', '津', '冀', '晋', '蒙', '辽', '吉', '黑'],
      ['苏', '浙', '皖', '闽', '赣', '鲁', '豫', '鄂', '湘'],
      ['桂', '琼', '渝', '川', '贵', '云', '藏'],
      ['陕', '甘', '青', '宁', '新'],
    ],
    // 车牌输入
    numbers: [
      ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9"],
      ["Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P"],
      ["A", "S", "D", "F", "G", "H", "J", "K", "L"],
      ["Z", "X", "C", "V", "B", "N", "M"]
    ],
    parkindex: '请选择停车场',
    carnum: [],
    showNewPower: false,
    KeyboardState: true
  },

  //  点击停车场组件确定事件  
  bindParkChange: function (e) {
    console.log(e.detail.value)
    console.log('这一条的值', this.data.parkArray[e.detail.value])
    this.setData({
      parkindex: e.detail.value
    })
  },

  // 查缴停车费
  paymentParking() {
    
  },

  // 选中点击设置
  bindChoose(e) {
    if (!this.data.carnum[6] || this.data.showNewPower) {
      var arr = [];
      arr[0] = e.target.dataset.val;
      this.data.carnum = this.data.carnum.concat(arr)
      this.setData({
        carnum: this.data.carnum
      })
    }
    if(this.data.carnum[6]){
      this.setData({
        showNewPower: true,
        KeyboardState: true
      })
    }
  },
  bindDelChoose() {
    if (this.data.carnum.length != 0) {
      this.data.carnum.splice(this.data.carnum.length - 1, 1);
      this.setData({
        carnum: this.data.carnum
      })
    }
  },
  showPowerBtn() {
    this.setData({
      showNewPower: true,
      KeyboardState: true
    })
  },
  closeKeyboard() {
    this.setData({
      KeyboardState: false
    })
  },
  openKeyboard() {
    this.setData({
      KeyboardState: true
    })
  },
  //清空按钮
  deleteAll(){
    this.setData({
      carnum:[],
    })
  },
  // 提交车牌号码
  submitNumber() {
    if(this.data.carnum.length<7){
      wx.showToast({
        title: '请输入完整的车牌!',
        icon:'none',
        duration: 2000,
      })
    }
    if (this.data.carnum[6]) {
           
      var carno=this.data.carnum;
      var carnonew=carno.join('');
      console.log(carnonew);
    }
  },
  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {
    this.setData({
      
    })
  },

  /**
   * 生命周期函数--监听页面初次渲染完成
   */
  onReady: function () {

  },

  /**
   * 生命周期函数--监听页面显示
   */
  onShow: function () {

  },

  /**
   * 生命周期函数--监听页面隐藏
   */
  onHide: function () {

  },

  /**
   * 生命周期函数--监听页面卸载
   */
  onUnload: function () {

  },

  /**
   * 页面相关事件处理函数--监听用户下拉动作
   */
  onPullDownRefresh: function () {

  },

  /**
   * 页面上拉触底事件的处理函数
   */
  onReachBottom: function () {

  },

  /**
   * 用户点击右上角分享
   */
  onShareAppMessage: function () {

  }
})