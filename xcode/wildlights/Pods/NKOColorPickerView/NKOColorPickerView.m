//
//  NKOColorPickerView.h
//  ColorPicker
//
//  Created by Carlos Vidal
//  Based on work by Fabián Cañas and Gilly Dekel
//


//UIImage category
@interface UIImage(NKO)

- (UIImage*)nko_tintImageWithColor:(UIColor*)tintColor;

@end

//NKOColorPickerView
#import "NKOColorPickerView.h"

CGFloat const NKOPickerViewGradientViewHeight           = 0.f;
CGFloat const NKOPickerViewGradientTopMargin            = 0.f;
CGFloat const NKOPickerViewDefaultMargin                = 10.f;
CGFloat const NKOPickerViewCrossHairshWidthAndHeight    = 38.f;

@interface NKOColorPickerView() {
	CGFloat currentHue;
	CGFloat currentSaturation;
}

@property (nonatomic, strong) UIImageView *hueSatImage;
@property (nonatomic, strong) UIView *crossHairs;

@end

@implementation NKOColorPickerView

- (id)initWithFrame:(CGRect)frame color:(UIColor*)color andDidChangeColorBlock:(NKOColorPickerDidChangeColorBlock)didChangeColorBlock
{
    self = [super init];
    
    if (self != nil){
        self.frame = frame;
        
        self->_color = color;
        self->_didChangeColorBlock = didChangeColorBlock;
    }
    
    return self;
}

- (void)willMoveToSuperview:(UIView *)newSuperview
{
    [super willMoveToSuperview:newSuperview];
    
    if (_color == nil){
        _color = [self _defaultTintColor];
    }
    
    [self.crossHairs setHidden:NO];
    
    [self setColor:_color];
    [self _updateCrosshairPosition];
}

- (void)layoutSubviews
{
    [super layoutSubviews];
    
    self.hueSatImage.frame = CGRectMake(NKOPickerViewDefaultMargin,
                                        NKOPickerViewDefaultMargin,
                                        CGRectGetWidth(self.frame) - (NKOPickerViewDefaultMargin*2),
                                        CGRectGetHeight(self.frame) - NKOPickerViewGradientViewHeight - NKOPickerViewDefaultMargin - NKOPickerViewGradientTopMargin);
    
    [self _updateCrosshairPosition];
}

#pragma mark - Public methods
- (void)setTintColor:(UIColor *)tintColor
{
    self.hueSatImage.layer.borderColor = tintColor.CGColor;
}

- (void)setColor:(UIColor *)newColor
{
    CGFloat hue, saturation;
    [newColor getHue:&hue saturation:&saturation brightness:nil alpha:nil];

    currentHue = hue;
    currentSaturation = saturation;
    [self _setColor:newColor];
    [self _updateCrosshairPosition];
}

#pragma mark - Private methods
- (void)_setColor:(UIColor *)newColor
{
    if (![_color isEqual:newColor]){
        CGFloat brightness;
        [newColor getHue:NULL saturation:NULL brightness:&brightness alpha:NULL];
        CGColorSpaceModel colorSpaceModel = CGColorSpaceGetModel(CGColorGetColorSpace(newColor.CGColor));
        if (colorSpaceModel==kCGColorSpaceModelMonochrome) {
            const CGFloat *c = CGColorGetComponents(newColor.CGColor);
            _color = [UIColor colorWithHue:0
                                saturation:0
                                brightness:c[0]
                                     alpha:1.0];
        }
        else{
            _color = [newColor copy];
        }
        
        if (self.didChangeColorBlock != nil){
            self.didChangeColorBlock(self.color);
        }
    }
}

- (void)_updateCrosshairPosition
{
    CGPoint hueSatPosition;
    
    hueSatPosition.x = (currentHue * self.hueSatImage.frame.size.width) + self.hueSatImage.frame.origin.x;
    hueSatPosition.y = (1.0-currentSaturation) * self.hueSatImage.frame.size.height + self.hueSatImage.frame.origin.y;
    
    self.crossHairs.center = hueSatPosition;
}

- (void)_updateHueSatWithMovement:(CGPoint)position
{
	currentHue = (position.x - self.hueSatImage.frame.origin.x) / self.hueSatImage.frame.size.width;
	currentSaturation = 1.0 -  (position.y - self.hueSatImage.frame.origin.y) / self.hueSatImage.frame.size.height;
    
	UIColor *_tcolor = [UIColor colorWithHue:currentHue
                                  saturation:currentSaturation
                                  brightness:1.0
                                       alpha:1.0];
    UIColor *gradientColor = [UIColor colorWithHue:currentHue
                                        saturation:currentSaturation
                                        brightness:1.0
                                             alpha:1.0];
	
    
    self.crossHairs.layer.backgroundColor = gradientColor.CGColor;
    [self _setColor:_tcolor];
}

- (UIColor*)_defaultTintColor
{
    return [[[[UIApplication sharedApplication] delegate] window] tintColor];
}

#pragma mark - Touch Handling methods
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	for (UITouch *touch in touches){
		[self dispatchTouchEvent:[touch locationInView:self]];
    }
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	for (UITouch *touch in touches){
		[self dispatchTouchEvent:[touch locationInView:self]];
	}
}

- (void)dispatchTouchEvent:(CGPoint)position
{
	if (CGRectContainsPoint(self.hueSatImage.frame,position)){
        self.crossHairs.center = position;
		[self _updateHueSatWithMovement:position];
	}
}

#pragma mark - Lazy loading
- (UIImageView*)hueSatImage
{
    if (self->_hueSatImage == nil){
        self->_hueSatImage = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"nko_colormap.png"]];
        self->_hueSatImage.frame = CGRectMake(NKOPickerViewDefaultMargin,
                                              NKOPickerViewDefaultMargin,
                                              CGRectGetWidth(self.frame) - (NKOPickerViewDefaultMargin*2),
                                              CGRectGetHeight(self.frame) - NKOPickerViewGradientViewHeight - NKOPickerViewDefaultMargin - NKOPickerViewGradientTopMargin);
        
        self->_hueSatImage.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
        
        self->_hueSatImage.layer.borderWidth = 1.f;
        self->_hueSatImage.layer.cornerRadius = 6.f;
        self->_hueSatImage.layer.borderColor = [self _defaultTintColor].CGColor;
        self->_hueSatImage.layer.masksToBounds = YES;
    }
    
    if (self->_hueSatImage.superview == nil){
        [self addSubview:self->_hueSatImage];
    }
    
    return self->_hueSatImage;
}

- (UIView*)crossHairs
{
    if (self->_crossHairs == nil){
        self->_crossHairs = [[UIView alloc] initWithFrame:CGRectMake(CGRectGetWidth(self.frame)*0.5,
                                                                     CGRectGetHeight(self.frame)*0.5,
                                                                     NKOPickerViewCrossHairshWidthAndHeight,
                                                                     NKOPickerViewCrossHairshWidthAndHeight)];
        
        self->_crossHairs.autoresizingMask = UIViewAutoresizingNone;
        
        UIColor *edgeColor = [UIColor colorWithWhite:0.9 alpha:0.8];
        
        self->_crossHairs.layer.cornerRadius = 19;
        self->_crossHairs.layer.borderColor = edgeColor.CGColor;
        self->_crossHairs.layer.borderWidth = 2;
        self->_crossHairs.layer.shadowColor = [UIColor blackColor].CGColor;
        self->_crossHairs.layer.shadowOffset = CGSizeZero;
        self->_crossHairs.layer.shadowRadius = 1;
        self->_crossHairs.layer.shadowOpacity = 0.5f;
    }
    
    if (self->_crossHairs.superview == nil){
        [self insertSubview:self->_crossHairs aboveSubview:self.hueSatImage];
    }
    
    return self->_crossHairs;
}

@end



//UIImage category
@implementation UIImage(NKO)

- (UIImage*)nko_tintImageWithColor:(UIColor*)tintColor
{
    UIGraphicsBeginImageContextWithOptions(self.size, NO, 0.0f);
    
    CGContextRef ctx = UIGraphicsGetCurrentContext();
    CGRect area = CGRectMake(0, 0, self.size.width, self.size.height);
    
    CGContextScaleCTM(ctx, 1, -1);
    CGContextTranslateCTM(ctx, 0, -area.size.height);
    CGContextSaveGState(ctx);
    CGContextClipToMask(ctx, area, self.CGImage);
    [tintColor set];
    CGContextFillRect(ctx, area);
    CGContextRestoreGState(ctx);
    CGContextSetBlendMode(ctx, kCGBlendModeMultiply);
    CGContextDrawImage(ctx, area, self.CGImage);
    
    UIImage *newImage = UIGraphicsGetImageFromCurrentImageContext();
    
    UIGraphicsEndImageContext();
    
    return newImage;
}

@end