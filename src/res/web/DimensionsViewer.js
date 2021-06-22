let design = {
    "$schema": "https://vega.github.io/schema/vega-lite/v5.json",
    "width": "container",
    "height": "container",
    "title": {
        "text": []
    },
    "config": {
        "axis": {
            "grid": true,
            "gridColor": "#dedede"
        }
    },
    "padding": 10,
    "autosize":
    {
        "type": "fit",
        "resize": true,
    },
    "data": {
        "name": "dimensions",
        "values": []
    },
    "layer": []
}

function getPointArea(radius) {
    let width = 2 * radius;
    return width * width;
}

function getRangeMark(channel, strokeWidth) {
    return {
        "mark": {
            "type": "errorband",
            "opacity": 0.005 * 0.3 * channel.opacity,
        },
        "transform": [
            {
                "filter": {
                    "field": "chn",
                    "equal": channel.index.toString()
                }
            }
        ],
        "encoding": {
            "x": channel.encoding.x,
            "y": {
                "field": "min",
                "type": "quantitative",
                "title": "Point value"
            },
            "y2": {
                "field": "max",
                "type": "quantitative",
                "title": "Point value"
            },
            "color": {
                "value": channel.color
            }
        }
    }
}

function getAggregateLineMark(channel, strokeWidth, strokeDash, selectionPointRadius, selectionPointThickness) {
    return {
        "transform": [
            {
                "filter": {
                    "field": "chn",
                    "equal": channel.index.toString()
                }
            }
        ],
        "encoding": {
            "x": channel.encoding.x,
            "y": {
                "field": "agg",
                "type": "quantitative",
                "title": "Point value"
            },
            "color": {
                "value": channel.color
            }
        },
        "layer": [
            {
                "mark": {
                    "type": "line",
                    "strokeWidth": strokeWidth,
                    "strokeDash": strokeDash,
                    "strokeJoin": "round",
                    "opacity": 0.01 * channel.opacity,
                }
            },
            {
                "transform": [{
                    "filter": {
                        "param": "hover",
                        "empty": false
                    }
                }],
                "mark": {
                    "type": "point",
                    "size": getPointArea(selectionPointRadius),
                    "opacity": 0.7,
                    "strokeWidth": selectionPointThickness
                }
            }
        ]
    }
}

function getAggregatePointsMark(channel, pointRadius) {
    return {
        "mark": {
            "type": "point",
            "fill": channel.color,
            "opacity": 0.01 * channel.opacity,
            "size": getPointArea(pointRadius)
        },
        "transform": [
            {
                "filter": {
                    "field": "chn",
                    "equal": channel.index.toString()
                }
            }
        ],
        "encoding": {
            "x": channel.encoding.x,
            "y": {
                "field": "agg",
                "type": "quantitative",
                "title": "Point value"
            },
            "color": {
                "value": channel.color
            }
        }
    }
}

function getStdDevLineMark(channel, field, strokeWidth, strokeDash) {
    return {
        "mark": {
            "type": "line",
            "strokeWidth": strokeWidth,
            "strokeDash": strokeDash,
            "strokeJoin": "round",
            "opacity": 0.01 * channel.opacity,
        },
        "transform": [
            {
                "filter": {
                    "field": "chn",
                    "equal": channel.index.toString()
                }
            }
        ],
        "encoding": {
            "x": channel.encoding.x,
            "y": {
                "field": field,
                "type": "quantitative",
                "title": "Point value"
            },
            "color": {
                "value": channel.color
            }
        }
    }
}

function getDashPattern(lineType) {
    if (lineType == "Solid")
        return [];

    if (lineType == "Dash")
        return [5, 5];

    if (lineType == "Dot")
        return [2, 2];

    if (lineType == "DashDot")
        return [5, 2, 2, 2];

    if (lineType == "DashDotDot")
        return [5, 2, 2, 2, 2, 2];

    return [];
}

function addChannel(design, channel) {
    design.data.values = design.data.values.concat(channel.dimensions);

    let halfPrimaryLineThickness = channel.primaryLineThickness / 2.0;

    if (channel.profileType >= 0) {
        design.layer.push(getAggregateLineMark(channel, channel.primaryLineThickness, getDashPattern(channel.primaryLineType), halfPrimaryLineThickness + 4, 2));

        if (channel.showPoints)
            design.layer.push(getAggregatePointsMark(channel, halfPrimaryLineThickness + 1));
    }
    
    if (channel.bandType > 0) {
        design.layer.push(getStdDevLineMark(channel, "v1", channel.secondaryLineThickness, getDashPattern(channel.secondaryLineType)));
        design.layer.push(getStdDevLineMark(channel, "v2", channel.secondaryLineThickness, getDashPattern(channel.secondaryLineType)));
    }
    
    if (channel.showRange)
        design.layer.push(getRangeMark(channel, 1));
}

function getDesign(spec) {

    design.data.values = [];
    design.layer = [];
    
    let titles = [];

    for (channelName in spec.channels) {
        let channel = spec.channels[channelName];

        titles.push(channel.datasetName);

        channel.encoding = {
            "x": {
                "field": spec.showDimensionNames ? 'dimName' : 'dimId',
                "type": spec.showDimensionNames ? 'ordinal' : 'nominal',
                "title": spec.showDimensionNames ? 'Dimension name' : 'Dimensions index',
                "axis": {
                    "labelAngle": spec.showDimensionNames ? -45 : 0
                }
            }
        }

        addChannel(design, channel);
    }

    design.layer.push({
        "transform": [{
            "pivot": "chn",
            "value": "min",
            "groupby": ["dimName"]
        }],
        "mark": "rule",
        "encoding": {
            "x": { "field": "dimName", "type": "nominal" },
            "opacity": {
                "condition": {
                    "value": 0.3,
                    "param": "hover",
                    "empty": false
                },
                "value": 0
            },
            "size": {
                "value": 2
            },
        },
        "params": [{
            "name": "hover",
            "select": {
                "type": "point",
                "fields": ["dimName"],
                "nearest": true,
                "on": "mouseover",
                "clear": "mouseout"
            }
        }]
    })
    
    design.title.text = `[${titles.join(', ')}]`;

    return design;
}