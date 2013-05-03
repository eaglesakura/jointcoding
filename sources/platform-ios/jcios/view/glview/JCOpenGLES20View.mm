#import "JCOpenGLES20View.h"
#import <QuartzCore/CAEAGLLayer.h>

using namespace ios;

@implementation JCOpenGLES20View

+ (Class) layerClass{
    return [CAEAGLLayer class];
}

/**
 * EGLの初期化を行う。
 */
- (void) initLayer {
    CAEAGLLayer *pLayer = ( CAEAGLLayer*)self.layer;
    pLayer.opaque = TRUE;
    pLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                 [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking,
                                 kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
}

- (id)init {
    self = [super init];
    assert(self);
    
    [self initLayer];
    
    return self;
}

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        [self initLayer];
    }
    return self;
}

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
    // Drawing code
}
*/

@end
