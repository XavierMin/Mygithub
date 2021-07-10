// pages/order/order.js
Page({

  /**
   * 页面的初始数据
   */
  data: {
    dates: '请选择日期',
    intime: '请选择入场时间',
    outtime: '请选择离场时间',
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
    carArray: [
      {carid: '0', carname: "苏E 12345"},
      {carid: '1', carname: "晥G 66666"},
      {carid: '2', carname: "鄂A 88888"} 
    ],
    couponArray: [
      {couponid: '0', couponname: "3元立减券(满10元可用)"},
      {couponid: '1', couponname: "5元立减券(满20元可用)"},
      {couponid: '2', couponname: "10元立减券(满30元可用)"} 
    ],
    parkindex: '请选择',
    carindex: '请选择',
    couponindex: '请选择优惠券',
  },
  
  //  点击停车场组件确定事件  
  bindParkChange: function (e) {
    console.log(e.detail.value)
    console.log('这一条的值', this.data.parkArray[e.detail.value])
    this.setData({
      parkindex: e.detail.value
    })
  },
  //  点击车牌组件确定事件  
  bindCarChange: function (e) {
    console.log(e.detail.value)
    console.log('这一条的值', this.data.carArray[e.detail.value])
    this.setData({
      carindex: e.detail.value
    })
  },
  //  点击日期组件确定事件  
  bindDateChange: function (e) {
    console.log(e.detail.value)
    this.setData({
      dates: e.detail.value
    })
  },
  //  点击入场时间组件确定事件  
  bindInTimeChange: function (e) {
    console.log(e.detail.value)
    this.setData({
      intime: e.detail.value
    })
  },
  //  点击离场时间组件确定事件  
  bindOutTimeChange: function (e) {
    console.log(e.detail.value)
    this.setData({
      outtime: e.detail.value
    })
  },
  //  点击优惠券组件确定事件  
  bindCouponChange: function (e) {
    console.log(e.detail.value)
    console.log('这一条的值', this.data.couponArray[e.detail.value])
    this.setData({
      couponindex: e.detail.value
    })
  },

  // 立即预约
  orderParking() {
    
  },
  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {

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